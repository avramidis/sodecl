//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef ODECL_SYSTEM_HPP
#define ODECL_SYSTEM_HPP

#include <vector>
#include <iostream>

#include <string>
#include <fstream>
#include <streambuf>

#include <time.h>

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

namespace odecl
{
	/// <summary>
	/// CLASS SYSTEM is the main class of the ODECL library.
	/// </summary>
	class system
	{
		/*********************************************************************************************
		VARIABLES SECTION
		*/
	public:

		cl_double *m_output;
		int m_output_size;

	private:

		/********************************************************************************************
		OPENCL HARDWARE SECTION VARIABLES
		*/
		// Number of OPENCL platforms
		cl_uint m_platform_count;

		// Vector which stores the odecl::platform objects. One object for each OpenCL platform.
		std::vector<odecl::platform*> m_platforms;

		// The index of the selected odecl::platform object in the m_platforms vector.
		cl_uint m_selected_platform;

		// The index of the selected odecl::device object in m_devices vector of selected platform.
		cl_uint m_selected_device;

		// Selected OpenCL device type.
		device_Type m_selected_device_type;

		/********************************************************************************************
		OPENCL SOFTWARE SECTION VARIABLES
		*/

		// OpenCL command contexts vector.
		std::vector<cl_context> m_contexts;

		// OpenCL command queues vector.
		std::vector<cl_command_queue> m_command_queues;

		// Char vector which stores the OpenCL kernel source string. TODO: store multiple kernel source strings.
		std::vector<char> m_kernel_sources;

		// OpenCL kernel string.
		char *m_source_str;

		// OpenCL kernels solvers path.
		string m_kernel_path_str;
		
		// OpenCL kernel string size.
		size_t m_source_size;

		// OpenCL programs vector.
		std::vector<cl_program> m_programs;

		// OpenCL kernels vector.
		std::vector<cl_kernel> m_kernels;

		// ODE solvers OpenCL buffers.
		cl_mem m_mem_t0;				// OpenCL memory buffer which stores the time value of each integration output step.
		cl_mem m_mem_y0;				// OpenCL memory buffer which stores the phase state of each integration output step.
		cl_mem m_mem_params;			// OpenCL memory buffer which stores the parameter values of each integration orbit of the ODE system.
		cl_mem m_mem_dt;				// OpenCL memory buffer which stores the time step for each integration orbit of the ODE system.

		/********************************************************************************************
		ODE SOLVERS SECTION VARIABLES
		*/

		char *m_ode_system_string;

		solver_Type m_solver;			// ODE solver type.
		double m_dt;					// Solver (initial) time step in seconds.
		double m_int_time;				// Time units to integrate the ODE system.
		int m_kernel_steps;				// Number of step the solver with perform in each kernel call.
		int m_num_dt_steps;				// Number of dt steps for the state of the ODE system to be saved.
		int m_list_size;				// Number of ODE system parameter combinations.
		int m_num_equat;				// Number of ODE system equations.
		int m_num_params;				// Number of ODE system parameters.
		output_Type m_output_type;		// Type of output (binary file or array).

		// arrays with the state of the system for each parameter combination
		cl_double *m_t0;				// Client memory buffer which stores the time value of each integration output step.
		cl_double *m_y0;				// Client memory buffer which stores the phase state of each integration output step.
		cl_double *m_params;			// Client memory buffer which stores the parameter values of each integration orbit of the ODE system.
		cl_double *m_dts;				// Client memory buffer which stores the time step for each integration orbit of the ODE system.


		/*********************************************************************************************
		FUNCTIONS SECTION
		*/
	public:

		/// <summary>
		/// Default constructor which initialises the odecl object. 
		/// </summary>
		/// <param name="kernel_path_str">Path to the OpenCL ODE solvers kernel source files.</param>
		/// <param name="ode_system_str">Path to the OpenCL ODE system source file.</param>
		/// <param name="solver">Type of the solver that will be used to integrate the ODE system.</param>
		/// <param name="dt">ODE solver time step size.</param>
		/// <param name="int_time">Length of time in seconds the the ODE system with be integrated.</param>
		/// <param name="kernel_steps">Number of steps the ODE solver will perform in each OpenCL device call.</param>
		/// <param name="num_equat">Number of equations of the ODE system.</param>
		/// <param name="num_params">Number of parameters of the ODE system.</param>
		/// <param name="list_size">Number of orbits to be integrated for the ODE system.</param>
		/// <param name="output_type">Specifies the location where the output of the integration of the ODE system will be stored.</param>
		system(string kernel_path_str, char *ode_system_str, solver_Type solver, double dt, double int_time, int kernel_steps, int num_equat, int num_params, int list_size, output_Type output_type)
		{
			// Set ODE Solver parameter values
			m_kernel_path_str = kernel_path_str;
			m_ode_system_string = ode_system_str;
			m_solver = solver;							// Choose the ODE solver.
			m_dt = dt;									// Solver (initial) time step in seconds.
			m_int_time = int_time;						// Integration time.
			m_kernel_steps = kernel_steps;				// Number of step the solver with perform in each kernel call.
			m_list_size = list_size;					// Number of different integrations (ODE system parameter combinations).
			m_num_equat = num_equat;					// Number of ODE system equations.
			m_num_params = num_params;					// Number of ODE system parameters.
			m_output_type = output_type;                // Type of output (Array or binary file).

			// Find the host's platforms
			// get the number of platforms in the system
			m_platform_count = get_platform_count();

			// create all odecl::platform objects
			create_platforms();

			// This is used for the variable time step solvers
			m_dts = new cl_double[m_list_size];
			for (int i = 0; i < m_list_size; i++)
			{
				m_dts[i] = m_dt;
			}
		}

		/// <summary>
		/// Destructor. Deletes all objects, arrays, pointers, etc. 
		/// </summary>
		~system()
		{
			delete m_output;
			delete m_source_str;

			//delete[] m_ode_system_string;  // This is a string literals. That means it has static storage duration (not dynamically allocated).
			delete m_t0;
			delete m_y0;
			delete m_params;
			delete m_dts;

			// Clean OpenCL memory buffers.
			clReleaseMemObject(m_mem_t0);
			clReleaseMemObject(m_mem_y0);
			clReleaseMemObject(m_mem_params);
			//clReleaseMemObject(m_mem_dt);

			// Clean OpenCL command queues.
			while (!m_command_queues.empty())
			{
				clReleaseCommandQueue((cl_command_queue)(m_command_queues.back()));
				m_command_queues.pop_back();
			}
			
			// Clean OpenCL kernels.
			while (!m_kernels.empty())
			{
				clReleaseKernel((cl_kernel)(m_kernels.back()));
				m_kernels.pop_back();
			}

			// Clean OpenCL programs.
			while (!m_programs.empty())
			{
				clReleaseProgram((cl_program)(m_programs.back()));
				m_programs.pop_back();
			}

			// Clean OpenCL contexts.
			while (!m_contexts.empty())
			{
				clReleaseContext((cl_context)(m_contexts.back()));
				m_contexts.pop_back();
			}
		}

		/******************************************************************************************
		HARDWARE SECTION
		*/

		/// <summary>
		/// Sets the odecl object to use the selected OpenCL device for the integration of the ODE model.
		/// </summary>
		/// <param name="platform_num">Index of selected OpenCL platform.</param>
		/// <param name="device_type">OpenCL device type.</param>
		/// <param name="device_num">Index of selected OpenCL device in the selected OpenCL platform.</param>
		void choose_device(int platform_num, device_Type device_type, int device_num)
		{
			// Check if selected platform exist
			if (platform_num<0 || platform_num>m_platform_count)
			{
				std::cout << "Selected platform number is out of bounds." << std::endl;
				return;
			}

			// Check if selected device exist
			if (device_num<0 || device_num>m_platforms[platform_num]->get_device_count())
			{
				std::cout << "Selected device number is out of bounds." << std::endl;
				return;
			}

			// Check if selected device type exist
			if (m_platforms[platform_num]->m_devices[device_num]->type() != (cl_device_type)device_type)
			{
				std::cout << "Selected device is not of the type selected." << std::endl;
				return;
			}

			std::cout << "Selected platform: " << m_platforms[platform_num]->name().c_str() << std::endl;
			std::cout << "Device name: " << m_platforms[platform_num]->m_devices[device_num]->name().c_str() << std::endl;
			//std::cout << "Device type: " << m_platforms[platform_num]->m_devices[device_num]->type() << std::endl;

			m_selected_platform = platform_num;
			m_selected_device = device_num;
			m_selected_device_type = device_type;
		}

		/******************************************************************************************
		SOFTWARE SECTION
		*/

	private:

		/******************************************************************************************
		HARDWARE SECTION
		*/

		/// <summary>
		/// Create all odecl::platform objects 
		/// </summary>
		void create_platforms()
		{

			//cl_uint platform_count = get_platform_count();
			cl_platform_id *cpPlatform = new cl_platform_id[m_platform_count];

			// get all platforms
			cl_int err = clGetPlatformIDs(m_platform_count, cpPlatform, NULL);
			if (err != CL_SUCCESS)
			{
				delete cpPlatform;
				//return NULL;
			}

			std::vector<cl_platform_id> m_platform_ids;

			// populate vector with cl_platform_ids
			for (int i = 0; i < m_platform_count; i++)
			{
				m_platform_ids.push_back(cpPlatform[i]);
			}

			//for (auto &i : m_platform_ids)
			//{
			//	m_platforms.push_back(new platform(i));
			//}

			for (int i = 0; i < m_platform_ids.size(); i++)
			{
				m_platforms.push_back(new platform(m_platform_ids.at(i)));
			}
		}

		/// <summary>
		/// Finds and returns the number of OpenCL platforms available.
		/// </summary>
		/// <returns>The number of OpenCL platforms available. Returns -1 if the operation fails.</returns>
		cl_uint get_platform_count()
		{
			cl_uint platform_count;

			// get platform count
			cl_int err = clGetPlatformIDs(10, NULL, &platform_count);

			if (err != CL_SUCCESS)
			{
				return -1;
			}

			return platform_count;
		}

		/// <summary>
		/// Sets the odecl object to use the CPU to integrate the ODE model. 
		/// </summary>
		/// <remarks>There could be an issue when selecting the OpenCL platform if there are more than one.</remarks>
		void get_CPU_device()
		{

		}

		/******************************************************************************************
		SOFTWARE SECTION
		*/

		/// <summary>
		/// Create OpenCL context for the selected OpenCL platform and OpenCL device.
		/// </summary>
		/// <returns>Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull.</returns>
		int create_context()
		{
			cl_int err;
			cl_context context = clCreateContext(NULL, 1, &(m_platforms[m_selected_platform]->m_devices[m_selected_device]->m_device_id), NULL, NULL, &err);
			if (err != CL_SUCCESS)
			{
				std::cout << "Error: Failed to create context! " << err << std::endl;
				return 0;
			}
			m_contexts.push_back(context);

			//std::cout << "Size of m_contexts : " << m_contexts.size() << std::endl;
			return 1;

		}

		/// <summary>
		/// Create OpenCL command queue for the selected OpenCL platform and OpenCL device. 
		/// </summary>
		/// <returns>Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull.</returns>
		int create_command_queue()
		{
			cl_context context = m_contexts[0];

			cl_device_id device = m_platforms[m_selected_platform]->m_devices[m_selected_device]->m_device_id;

			cl_int err;
			cl_command_queue commands = clCreateCommandQueue(context, device, NULL, &err);
			if (err != CL_SUCCESS)
			{
				std::cout << "Error: Failed to create command queue!" << std::endl;
				return 0;
			}

			m_command_queues.push_back(commands);

			return 1;
		}

		/******************************************************************************************
		ODE SOLVERS SECTION
		*/

	private:

		void read_kernel_file(char* filename)
		{
			// Read the kernel files that include the runge kutta solve
			std::ifstream t(filename);
			std::string str;

			t.seekg(0, std::ios::end);
			//cout << "Kernel string size: " << t.tellg() << endl;

			str.reserve(t.tellg());

			t.seekg(0, std::ios::beg);

			str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

			t.close();

			m_source_size = str.length();

			for (int i = 0; i < m_source_size; i++)
			{
				m_kernel_sources.push_back(str[i]);
			}
		}

		void add_string_to_kernel_sources(string str)
		{
			int m_source_size = str.length();

			for (int i = 0; i < m_source_size; i++)
			{
				m_kernel_sources.push_back(str[i]);
			}
		}

		// convert double x to string s
		string double2string(double x)
		{
			std::string s;
			{
				std::ostringstream ss;
				ss << x;
				s = ss.str();
			}
			return s;
		}

		int create_kernel_string()
		{
			// Create the parameters section of the kernel string

			cout << "Create OpenCL kernel string" << endl;

			// Kernel steps
			add_string_to_kernel_sources("#define _numsteps_ ");
			add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_kernel_steps)));
			add_string_to_kernel_sources("\n");

			cout << "Kernel steps done" << endl;

			// ODE solver time size
			add_string_to_kernel_sources("#define _m_dt ");
			//add_string_to_kernel_sources(std::to_string(static_cast<long double>(m_dt)));
			add_string_to_kernel_sources(double2string(m_dt));
			add_string_to_kernel_sources("\n");

			cout << "ODE solver time size" << endl;

			// ODE system number of equations
			add_string_to_kernel_sources("#define _numeq_ ");
			add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_num_equat)));
			add_string_to_kernel_sources("\n");

			// ODE system number of parameters
			add_string_to_kernel_sources("#define _numpar_ ");
			add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_num_params)));
			add_string_to_kernel_sources("\n");

			double epsilon1 = 1e-7;
			// Implicit Euler Newton-Raphson epsilon1 value
			add_string_to_kernel_sources("#define _epsilon1_ ");
			add_string_to_kernel_sources(double2string(epsilon1));
			add_string_to_kernel_sources("\n");

			cout << "ODE system number of parameters" << endl;

			// Read the ODE system functions
			read_kernel_file(m_ode_system_string);
			add_string_to_kernel_sources("\n");

			std::vector<char> kernelpath_char;
			string kernelpath = m_kernel_path_str;

			std::cout << "The kernal path is: " << kernelpath << std::endl;

			// Choose the solver.
			switch (m_solver){
			case solver_Type::Euler:
				cout << "Read the Euler solver" << endl;
				kernelpath.append("/euler.cl");
				//read_kernel_file("euler.cl");	// Euler
				break;
			case solver_Type::RungeKutta:
				cout << "Read the Runge-Kutta solver" << endl;
				kernelpath.append("/rk4.cl");
				//read_kernel_file("rk4.cl");	// Runge-Kutta
				break;
			case solver_Type::ImplicitEuler:
				cout << "Read the Implicit Euler solver" << endl;
				kernelpath.append("/ie.cl");
				//read_kernel_file("ie.cl");	// Implicit Euler
				break;
			default:
				std::cout << "No valid solver chosen!" << std::endl;
			}

			for (int i = 0; i < kernelpath.size(); i++)
			{
				kernelpath_char.push_back(kernelpath[i]);
			}
			kernelpath_char.push_back('\0');

			cout << &*kernelpath_char.begin() << endl;
			
			kernelpath_char.shrink_to_fit();

			read_kernel_file(&*kernelpath_char.begin());

			add_string_to_kernel_sources("\n");

			// Read the solver 
			read_kernel_file("solver_caller.cl");
			add_string_to_kernel_sources("\n");

			// Print the string
			// cout << m_kernel_sources.data() << endl;

			m_source_size = m_kernel_sources.size();

			// This is for debug purpose
			std::ofstream out("Generated_kernel.txt");
			out << m_kernel_sources.data();
			out.close();

			return 1;
		}

		/// <summary>
		/// Create OpenCL program for the selected OpenCL platform, OpenCL device and created OpenCL context.
		/// </summary>
		/// <returns>Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull.</returns>
		int create_program()
		{
			const char *srcptr[] = { m_kernel_sources.data() };

			//std::cout << "Size of string is: " << m_source_size << std::endl;

			cl_int err;
			cl_program program = clCreateProgramWithSource(m_contexts.at(0), 1, srcptr, (const size_t *)&m_source_size, &err);
			if (err != CL_SUCCESS)
			{
				return 0;
			}

			// Create OpenCL Program
			m_programs.push_back(program);

			return 1;
		}

		/// <summary>
		/// Build OpenCL program for the selected OpenCL device.
		/// </summary>
		/// <returns>Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull.</returns>
		int build_program()
		{
			cl_device_id device_id = m_platforms[m_selected_platform]->m_devices[m_selected_device]->m_device_id;

			// Build options - TODO: will be part of the options

			//const char * options = "-D KHR_DP_EXTENSION -x clc++ -cl-mad-enable";
			//const char * options = "-D KHR_DP_EXTENSION -cl-opt-disable -cl-mad-enable";
			//const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-unsafe-math-optimizations";
			//const char * options = "-D KHR_DP_EXTENSION -cl-opt-disable";
			//const char * options = "-D KHR_DP_EXTENSION -cl-std=CL2.0 -cl-uniform-work-group-size";
			//const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-no-signed-zeros -cl-finite-math-only";
			//const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-std=CL2.0";
			//const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math";
			//const char * options = "-D KHR_DP_EXTENSION -cl-std=CL2.0";
			//const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-std=CL2.0";
			//const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math";
			//const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-std=CL2.0";
			//const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-strict-aliasing -cl-single-precision-constant -cl-no-signed-zeros -cl-denorms-are-zero";
			//const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-nv-maxrregcount=200 -cl-nv-verbose -cl-mad-enable -cl-fast-relaxed-math -cl-no-signed-zeros -cl-strict-aliasing";
			//const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-nv-maxrregcount=200 -cl-nv-verbose -cl-mad-enable -cl-no-signed-zeros -cl-strict-aliasing";
			//const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math  -cl-nv-maxrregcount=90";
			//const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-nv-opt-level=2";
			//const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math -cl-nv-allow-expensive-optimizations";
			const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math";
			//const char * options = "";

			cl_int err = clBuildProgram(m_programs[0], 1, &device_id, options, NULL, NULL);
			if (err != CL_SUCCESS)
			{
				size_t len;
				char buffer[2048];

				std::cout << "Error: " << err << std::endl;

				std::cout << "Error: Failed to build program executable!" << std::endl;
				clGetProgramBuildInfo(m_programs[0], device_id, CL_PROGRAM_BUILD_LOG,
					sizeof(buffer), buffer, &len);
				std::cout << buffer << std::endl;
				//system("pause");
				return 0;
			}

			return 1;
		}

		/// <summary>
		/// Create the OpenCL kernel.
		/// </summary>
		/// <param name="kernelname">Name of the OpenCL kernel.</param>
		/// <param name="program">OpenCL program.</param>
		/// <returns>Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull.</returns>
		int create_kernel(char* kernelname, cl_program program)
		{
			cl_int err;

			cl_kernel kernel = clCreateKernel(program, kernelname, &err);
			if (!kernel || err != CL_SUCCESS)
			{
				std::cout << "Error: Failed to create compute kernel!" << std::endl;
				return 0;
			}

			m_kernels.push_back(kernel);

			return 1;
		}

		/// <summary>
		/// Creates the buffers for specific context and command queue.
		/// </summary>
		/// <param name="context">The context is the cl_context.</param>
		/// <param name="commands">The commands is the cl_command_queue.</param>
		/// <param name="list_size">The list_size is the size of the buffers.</param>
		/// <param name="equat_num">The equat_num is number of equations.</param>
		/// <param name="param_num">The param_num is the number of parameters.</param>
		int create_buffers(cl_context context, cl_command_queue commands, int list_size, int equat_num, int param_num)
		{
			cl_int errcode;
			// Create OpenCL device memory buffers
			m_mem_t0 = clCreateBuffer(context, CL_MEM_READ_WRITE, list_size * sizeof(cl_double), NULL, &errcode);
			if (errcode != CL_SUCCESS)
			{
				return 0;
			}

			m_mem_y0 = clCreateBuffer(context, CL_MEM_READ_WRITE, list_size * sizeof(cl_double)*equat_num, NULL, &errcode);
			if (errcode != CL_SUCCESS)
			{
				return 0;
			}

			m_mem_params = clCreateBuffer(context, CL_MEM_READ_ONLY, list_size * sizeof(cl_double)*param_num, NULL, &errcode);
			if (errcode != CL_SUCCESS)
			{
				return 0;
			}

			return 1;
		}

		/// <summary>
		/// Create OpenCL device memory buffer for the dt.
		/// </summary>
		/// <param name="context">OpenCL context.</param>
		/// <param name="commands">OpenCL command queue.</param>
		/// <param name="list_size">Number of orbits which the ODE system with be integrated for.</param>
		/// <returns>Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull.</returns>
		int create_dt_buffer(cl_context context, cl_command_queue commands, int list_size)
		{
			cl_int errcode;
			m_mem_dt = clCreateBuffer(context, CL_MEM_READ_WRITE, list_size * sizeof(cl_double), NULL, &errcode);
			if (errcode != CL_SUCCESS)
			{
				return 0;
			}

			return 1;
		}

		/// <summary>
		/// Write data in the OpenCL memory buffers.
		/// </summary>
		/// <param name="commands">OpenCL command queue.</param>
		/// <param name="list_size">Number of data points to be written in the OpenCL memory buffers.</param>
		/// <param name="equat_num">Number of equations of the ODE system.</param>
		/// <param name="param_num">Number of parameters of the ODE system.</param>
		/// <returns>Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull.</returns>
		int write_buffers(cl_command_queue commands, int list_size, int equat_num, int param_num)
		{
			int err = 0;
			err |= clEnqueueWriteBuffer(commands, m_mem_t0, CL_TRUE, 0, list_size * sizeof(cl_double), m_t0, 0, NULL, NULL);
			err |= clEnqueueWriteBuffer(commands, m_mem_y0, CL_TRUE, 0, list_size * sizeof(cl_double)*equat_num, m_y0, 0, NULL, NULL);
			err |= clEnqueueWriteBuffer(commands, m_mem_params, CL_TRUE, 0, list_size * sizeof(cl_double)*param_num, m_params, 0, NULL, NULL);

			if (err != CL_SUCCESS)
			{
				std::cout << "Error: Failed to write to source array!" << std::endl;
				return 0;
			}

			return 1;
		}

		/// <summary>
		/// Write the initial ODE solver time steps for each ODE system orbit.
		/// </summary>
		/// <param name="commands">OpenCL command queue.</param>
		/// <param name="list_size">Number of data points to be written in the OpenCL memory buffers.</param>
		/// <returns>Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull.</returns>
		int write_dt_buffer(cl_command_queue commands, int list_size)
		{
			int err = 0;
			err |= clEnqueueWriteBuffer(commands, m_mem_dt, CL_TRUE, 0, list_size * sizeof(cl_double), m_dts, 0, NULL, NULL);

			if (err != CL_SUCCESS)
			{
				std::cout << "Error: Failed to write to source array!" << std::endl;
				return 0;
			}

			return 1;
		}

		void input_data()
		{

		}

		/// <summary>
		/// Sets OpenCL kernel arguments.
		/// </summary>
		/// <param name="kernel">OpenCL kernel.</param>
		/// <returns>Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull.</returns>
		int set_kernel_args(cl_kernel kernel)
		{
			// Set the arguments to the compute kernel
			cl_int err = 0;
			err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &m_mem_t0);
			err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &m_mem_y0);
			err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &m_mem_params);

			//err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &m_mem_dt);

			if (err != CL_SUCCESS)
			{
				std::cout << "Error: Failed to set kernel arguments! " << err << std::endl;
				return 0;
			}
			return 1;
		}

	public:

		void set_t0(cl_double *t0)
		{
			m_t0 = t0;
		}

		void set_y0(cl_double *y0)
		{
			m_y0 = y0;
		}

		void set_params(cl_double *params)
		{
			m_params = params;
		}

		/// <summary>
		/// Reads data from a txt file and saves the data in an array.
		/// </summary>
		/// <param name="filename">Filename to read the data from.</param>
		/// <param name="data">Pointer to cl_double array to store the read data to.</param>
		/// <returns>Returns 1 if the read of the data is successful or 0 if unsuccessful.</returns>
		int read_data_from_file(char *filename, cl_double *data)
		{
			// a string to store line of text
			string textLine;
			// try to open a file
			ifstream ifs(filename, ifstream::in);

			int j = 0;
			if (ifs.good())
			{
				while (!ifs.eof())
				{
					// read line of text
					getline(ifs, textLine);

					if (!textLine.empty())
					{
						istringstream iss(textLine);

						vector<string> strs;
						copy(istream_iterator<string>(iss),
							istream_iterator<string>(),
							back_inserter(strs));

						for (std::vector<string>::size_type i = 0; i != strs.size(); i++)
						{
							data[j] = atof(strs[i].c_str());
							j++;
						}
					}
				}
				ifs.close();
				return 1;
			}
			else
			{
				cout << "ERROR: can't open " << filename << " file." << endl;
				return 0;
			}

			return 1;
		}

		/// <summary>
		/// Executes the ODE solver on the selected OpenCL device.
		/// </summary>
		void run_ode_solver()
		{
			// Output binary files
			std::ofstream g_stream;
			if (m_output_type == odecl::output_Type::File)
			{
				g_stream.open("g.bin", std::ios::binary | std::ios::app | std::ios::out);
			}

			cl_double *t_out = new cl_double[m_list_size];
			cl_double *g = new cl_double[m_list_size * m_num_equat];

			size_t global = size_t(m_list_size);
			size_t local;

			if (m_selected_device_type == device_Type::CPU)
			{
				local = size_t(8);
			}
			else
			{
				local = size_t(64);
				//local = size_t(256);
			}
			//local = 256;
			cout << "The local group size is: " << local << endl;

			//cl_double *output_data = new cl_double[m_list_size * m_int_time / m_dt / m_kernel_steps];
			m_output_size = m_list_size * m_int_time / m_dt / m_kernel_steps;

			timer start_timer;

			int count = 0;
			std::cout << "Running kernel.." << std::endl;
			cl_int err;
			for (int j = 0; j < (m_int_time / m_dt / m_kernel_steps); j++)
			{
				// Read buffer g into a local list
				//err = clEnqueueReadBuffer(m_command_queues[0], m_mem_t0, CL_TRUE, 0, m_list_size * sizeof(cl_double), t_out, 0, NULL, NULL);
				err = clEnqueueReadBuffer(m_command_queues[0], m_mem_y0, CL_TRUE, 0, m_list_size * sizeof(cl_double)* m_num_equat, g, 0, NULL, NULL);
				//err = clEnqueueNDRangeKernel(m_command_queues[0], m_kernels[0], 1, NULL, &global, &local, 0, NULL, NULL);
				err = clEnqueueNDRangeKernel(m_command_queues[0], m_kernels[0], 1, NULL, &global, NULL, 0, NULL, NULL);
				if (err)
				{
					std::cout << "Error: Failed to execute kernel!" << std::endl;
					return;
				}
				clFlush(m_command_queues[0]);
				//clFinish(m_command_queues[0]);

				// Save data to disk or to data array - all variables
				for (int jo = 1; jo <= 6; jo++)
				{
					for (int ji = jo - 1; ji < m_list_size*m_num_equat; ji = ji + m_num_equat)
					{
						if (m_output_type == odecl::output_Type::File)
						{
							g_stream.write((char *)(&g[ji]), sizeof(cl_double));
						}
						//output_data[count] = g[ji];
						//count++;
						//cout << g[ji] << endl;
					}
				}
			}
			
			// Save the data from the last kernel call.
			err = clEnqueueReadBuffer(m_command_queues[0], m_mem_y0, CL_TRUE, 0, m_list_size * sizeof(cl_double)* m_num_equat, g, 0, NULL, NULL);
			// Save data to disk or to data array - all variables
			for (int jo = 1; jo <= 6; jo++)
			{
				for (int ji = jo - 1; ji < m_list_size*m_num_equat; ji = ji + m_num_equat)
				{
					if (m_output_type == odecl::output_Type::File)
					{
						g_stream.write((char *)(&g[ji]), sizeof(cl_double));
					}
					//output_data[count] = g[ji];
					//count++;
					//cout << g[ji] << endl;
				}
			}

			//m_output = output_data;

			start_timer.stop_timer();

			if (m_output_type == odecl::output_Type::File)
			{
				g_stream.close();
			}

			//delete t_out;
			delete g;
		}

		/// <summary>
		/// Setups the selected ODE solver OpenCL kernel source.
		/// </summary>
		/// <returns>Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull.</returns>
		int setup_ode_solver()
		{
			// To create a cl string with the program to run
			// 
			if (create_kernel_string() == 0)
			{
				std::cout << "Kernel code creation failed." << std::endl;
				return 0;
			}

			std::cout << "Kernel code creation successed." << std::endl;

			//read_kernel_file("test_kernel.cl"); // TODO: make the pro
			//std::cout << "Kernel read." << std::endl;

			// MATLAB mex file has a problem
			// Check previous variables for problems

			if (create_context() == 0)
			{
				std::cout << "Context creation failed." << std::endl;
				return 0;
			}
			std::cout << "Context created." << std::endl;

			if (create_program() == 0)
			{
				std::cout << "Failed to create OpenCL program from source." << std::endl;
				return 0;
			}
			std::cout << "Program created." << std::endl;

			if (build_program() == 0)
			{
				std::cout << "Failed to build kernel." << std::endl;
				return 0;
			}
			std::cout << "Program build." << std::endl;

			if (create_kernel("solver_caller", m_programs[0]) == 0)
			{
				std::cout << "Failed to create kernel." << std::endl;
				return 0;
			}
			std::cout << "Kernel created." << std::endl;

			if (create_command_queue() == 0)
			{
				std::cout << "Failed to create command queue." << std::endl;
				return 0;
			}
			std::cout << "Command queue created." << std::endl;

			if (create_buffers(m_contexts[0], m_command_queues[0], m_list_size, m_num_equat, m_num_params) == 0)
			{
				std::cout << "Failed to create the OpenCL buffers." << std::endl;
				return 0;
			}
			std::cout << "Buffers created." << std::endl;

			////create_test_data(list_size, equat_num, param_num);

			if (write_buffers(m_command_queues[0], m_list_size, m_num_equat, m_num_params) == 0)
			{
				std::cout << "Failed to write the data to the OpenCL buffers." << std::endl;
				return 0;
			}
			std::cout << "Data written to buffers." << std::endl;

			//if (create_dt_buffer(m_contexts[0], m_command_queues[0], m_list_size) == 0)
			//{
			//	std::cout << "Failed to create the OpenCL dt buffer." << std::endl;
			//	return 0;
			//}
			//std::cout << "dt buffer created." << std::endl;

			//if (write_dt_buffer(m_command_queues[0], m_list_size) == 0)
			//{
			//	std::cout << "Failed to write the data to the OpenCL dt buffer." << std::endl;
			//	return 0;
			//}
			//std::cout << "Data written to dt buffer." << std::endl;



			//std::cout << "Number of kernels: " << m_kernels.size() << std::endl;

			// assign the inputs to kernel
			if (set_kernel_args(m_kernels[0]) == 0)
			{
				std::cout << "Failed to set kernel arguments." << std::endl;
				return 0;
			}
			std::cout << "Kernel arguments set." << std::endl;

			run_ode_solver();

			return 1;
		}
	};

}


#endif // ODECL_SYSTEM_HPP