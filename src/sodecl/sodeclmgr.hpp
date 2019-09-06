//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef sodecl_system_HPP
#define sodecl_system_HPP

#include <vector>
#include <list>
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
#include <random>

using namespace std;

namespace sodecl {
    /**
    *  Manager class of sodecl.
    */
    class sodeclmgr {
        /*********************************************************************************************
        VARIABLES SECTION
        */
    public:

        std::vector <double> m_output;         /**< Integration output */
        int m_output_size;    /**< Output size */

    private:

        /********************************************************************************************
        OPENCL HARDWARE SECTION VARIABLES
        */

        cl_uint m_platform_count;            /**< Number of OPENCL platforms */
        std::vector<sodecl::platform *> m_platforms;                /**< Vector which stores the sodecl::platform objects. One object for each OpenCL platform */
        cl_uint m_selected_platform;        /**< The index of the selected sodecl::platform object in the m_platforms vector */
        cl_uint m_selected_device;            /**< The index of the selected sodecl::device object in m_devices vector of selected platform */
        device_Type m_selected_device_type;        /**< Selected OpenCL device type */

        /********************************************************************************************
        OPENCL SOFTWARE SECTION VARIABLES
        */

        std::vector <cl_context> m_contexts;                /**< OpenCL command contexts vector */
        std::vector <cl_command_queue> m_command_queues;        /**< OpenCL command queues vector */
        std::vector<char> m_kernel_sources;        /**< Char vector which stores the OpenCL kernel source string. @todo store multiple kernel source strings */
        std::string m_build_options_str;    /**< Char vector which stores the OpenCL build options string */
        std::vector <build_Option> m_build_options;        /**< build_Option vector which stores the OpenCL build options selection */
        char *m_source_str;            /**< OpenCL kernel string */
        string m_kernel_path_str;        /**< OpenCL kernels solvers path */
        size_t m_source_size;            /**< OpenCL kernel string size */
        std::vector <cl_program> m_programs;                /**< OpenCL programs vector */
        std::vector <cl_kernel> m_kernels;                /**< OpenCL kernels vector */
        int m_local_group_size;        /**< OpenCL device local group size */

        // ODE solvers OpenCL buffers
        cl_mem m_mem_t0;                /**< OpenCL memory buffer which stores the time value of each integration output step */
        cl_mem m_mem_y0;                /**< OpenCL memory buffer which stores the phase state of each integration output step */
        cl_mem m_mem_detterm;            /**< OpenCL memory buffer which stores the temporary phase state of each integration output step */
        cl_mem m_mem_stoch;            /**< OpenCL memory buffer which stores the stoch phase state of each integration output step */
        cl_mem m_mem_params;            /**< OpenCL memory buffer which stores the parameter values of each integration orbit of the ODE system */
        cl_mem m_mem_dt;                /**< OpenCL memory buffer which stores the time step for each integration orbit of the ODE system */
        cl_mem m_mem_rcounter;            /**< OpenCL memory buffer which stores the counters for each workitem for the random number generation */
        cl_mem m_mem_noise;            /**< OpenCL memory buffer which stores the noise for each workitem for the random number generation */
        string m_outputfile_str;        /**< Path to the results output file */

        /********************************************************************************************
        SODE SOLVERS SECTION VARIABLES
        */

        char *m_sode_system_string;            /**< ODE or SDE system OpenCL function */
        solver_Type m_solver;                  /**< ODE solver type */
        double m_dt;                           /**< Solver (initial) time step in seconds */
        double m_int_time;                     /**< Time units to integrate the ODE system */
        int m_kernel_steps;                    /**< Number of step the solver with perform in each kernel call */
        int m_num_dt_steps;                    /**< Number of dt steps for the state of the ODE system to be saved */
        int m_list_size;                       /**< Number of ODE system parameter combinations */
        int m_num_equat;                       /**< Number of ODE system equations */
        int m_num_params;                      /**< Number of ODE system parameters */
        int m_num_noi;                         /**< Number of noise processes */
        output_Type m_output_type;             /**< Type of output (binary file or array) */
        int *m_outputPattern;                  /**< Array which holds the output pattern. E.g. if the system has 3 equations the array {1,2,3}
													will write to the output file the results of the 1st then 2nd and then 3rd state variable */

        // arrays with the state of the system for each parameter combination
        cl_double *m_t0;            /**< Client memory buffer which stores the time value of each integration output step */
        cl_double *m_y0;            /**< Client memory buffer which stores the phase state of each integration output step */
        cl_double *m_params;        /**< Client memory buffer which stores the parameter values of each integration orbit of the ODE system */
        cl_double *m_dts;            /**< Client memory buffer which stores the time step for each integration orbit of the ODE system */
        cl_double *m_rcounter;        /**< Counter that counts the number of calls to the random number generator */

        // Log mechanisms
        clog *m_log;                /**< Pointer for log */

        /*********************************************************************************************
        FUNCTIONS SECTION
        */
    public:

        /**
        *  Default constructor which initialises the sodecl object.
        *
        * @param  kernel_path_str		Path to the OpenCL ODE solvers kernel source files
        * @param  sode_system_str		Path to the OpenCL ODE system source file
        * @param  solver				Type of the solver that will be used to integrate the ODE system
        * @param  dt					ODE solver time step size
        * @param  int_time				Length of time in seconds the the ODE system with be integrated
        * @param  kernel_steps			Number of steps the ODE solver will perform in each OpenCL device call
        * @param  num_equat				Number of equations of the ODE system
        * @param  num_params			Number of parameters of the ODE system
        * @param  list_size				Number of orbits to be integrated for the ODE system
        * @param  output_type			Specifies the location where the output of the integration of the ODE system will be stored
        */
        sodeclmgr(string kernel_path_str, char *sode_system_str, solver_Type solver, double dt, double int_time,
                  int kernel_steps, int num_equat, int num_params, int num_noi, int list_size,
                  output_Type output_type) {
            // Initialise the clog object
            m_log = clog::getInstance();

            // Set ODE Solver parameter values
            m_kernel_path_str = kernel_path_str;
            m_sode_system_string = sode_system_str;
            m_solver = solver;
            m_dt = dt;
            m_int_time = int_time;
            m_kernel_steps = kernel_steps;
            m_list_size = list_size;
            m_num_equat = num_equat;
            m_num_params = num_params;
            m_num_noi = num_noi;
            m_output_type = output_type;

            m_outputfile_str = "sodecloutput.bin";

            if (m_num_noi > 0) {
                double lower_bound = -100000000000;
                double upper_bound = 100000000000;
                std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
                std::random_device rd;
                std::mt19937 gen(rd());

                m_rcounter = new double[m_list_size];
                for (int i = 0; i < m_list_size; i++) {
                    m_rcounter[i] = unif(gen);
                }
            }

            m_outputPattern = new int[m_num_equat];
            for (int i = 0; i < m_num_equat; i++) {
                //m_outputPattern[i] = i+1;
                m_outputPattern[i] = 1;
            }
            m_outputPattern[0] = 1;

            // Find the host's platforms
            // get the number of platforms in the system
            m_platform_count = get_platform_count();
            if ((int) m_platform_count == -1) {
                cerr << "Error getting OpenCL planform number!" << endl;
            }

            // create all sodecl::platform objects, one for each OpenCL platform
            create_platforms();

            // Add default OpenCL build options
            //m_build_options.push_back(build_Option::FastRelaxedMath);
            //m_build_options.push_back(build_Option::stdCL20);
            //m_build_options.push_back(build_Option::stdCL21);

            m_local_group_size = 0;

            //m_log->toFile();
        }

        /**
        *  Destructor. Deletes all objects, arrays, pointers, etc.
        */
        ~sodeclmgr() {
            // delete m_sode_system_string;
            // delete m_output;
            // delete m_source_str;

            // delete[] m_sode_system_string;  // This is a string literals. That means it has static storage duration (not dynamically allocated).

            /////////////////////////////////////////////////

            // delete m_t0;
            // delete m_y0;
            // delete m_params;
            ////delete m_dts;
            if (m_num_noi > 0) {
                delete[] m_rcounter;
            }
            delete[] m_outputPattern;

            // Clean OpenCL memory buffers.
            clReleaseMemObject(m_mem_t0);
            clReleaseMemObject(m_mem_y0);
            clReleaseMemObject(m_mem_params);
            if (m_num_noi > 0) {
                clReleaseMemObject(m_mem_rcounter);
            }
            //clReleaseMemObject(m_mem_dt);

            // Clean OpenCL command queues.
            while (!m_command_queues.empty()) {
                clReleaseCommandQueue((cl_command_queue) (m_command_queues.back()));
                m_command_queues.pop_back();
            }

            // Clean OpenCL kernels.
            while (!m_kernels.empty()) {
                clReleaseKernel((cl_kernel) (m_kernels.back()));
                m_kernels.pop_back();
            }

            // Clean OpenCL programs.
            while (!m_programs.empty()) {
                clReleaseProgram((cl_program) (m_programs.back()));
                m_programs.pop_back();
            }

            // Clean OpenCL contexts.
            while (!m_contexts.empty()) {
                clReleaseContext((cl_context) (m_contexts.back()));
                m_contexts.pop_back();
            }

            for (auto i : m_platforms) {
                delete i;
            }

            m_platforms.clear();

            // The executable exited normally
            m_log->writeExitStatusFile(0, "Normal.");
            delete m_log;
        }

        /******************************************************************************************
        HARDWARE SECTION
        */

        /**
        *  Sets the sodecl object to use the selected OpenCL device for the integration of the ODE model.
        *
        * @param	platform_num	Index of selected OpenCL platform
        * @param	device_type		OpenCL device type
        * @param	device_num		Index of selected OpenCL device in the selected OpenCL platform
        *
        * @return	output_type		Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int choose_device(cl_uint platform_num, device_Type device_type, cl_uint device_num) {
            // Check if selected platform exist
            if (platform_num < 0 || platform_num > m_platform_count) {
                //cerr << "Selected platform number is out of bounds." << std::endl;
                m_log->write("Selected platform number is out of bounds.\n");

                return 0;
            }

            // Check if selected device exist
            if (device_num < 0 || device_num > m_platforms[platform_num]->get_device_count()) {
                //cerr << "Selected device number is out of bounds." << std::endl;
                m_log->write("Selected device number is out of bounds.\n");
                return 0;
            }

            // If selected device type is not ALL (OpenCL type) check if selected device type exist
            if ((cl_device_type) device_type != (cl_device_type) device_Type::ALL) {
                if (m_platforms[platform_num]->m_devices[device_num]->type() != (cl_device_type) device_type) {
                    //cerr << "Selected device is not of the type selected." << std::endl;
                    m_log->write("Selected device is not of the type selected.\n");
                    return 0;
                }
            }

            //std::cout << "Selected platform: " << m_platforms[platform_num]->name().c_str() << std::endl;
            //std::cout << "Device name: " << m_platforms[platform_num]->m_devices[device_num]->name().c_str() << std::endl;
            ////std::cout << "Device type: " << m_platforms[platform_num]->m_devices[device_num]->type() << std::endl;

            m_log->write("Selected platform name: ");
            m_log->write(m_platforms[platform_num]->name().c_str());
            m_log->write("\n");

            m_log->write("Selected device name: ");
            m_log->write(m_platforms[platform_num]->m_devices[device_num]->name().c_str());
            m_log->write("\n");

            m_log->write("Selected device OpenCL version: ");
            m_log->write(m_platforms[platform_num]->m_devices[device_num]->version().c_str());
            m_log->write("\n");

            m_selected_platform = platform_num;
            m_selected_device = device_num;
            m_selected_device_type = device_type;

            return 1;
        }

        /**
        *  Sets the number of work items in each group.
        *
        * @param	local_group_size	Number of work items for each group
        */
        void set_local_group_size(int local_group_size) {
            m_local_group_size = local_group_size;
        }

        /******************************************************************************************
        SOFTWARE SECTION
        */

    private:

        /******************************************************************************************
        HARDWARE SECTION
        */

        /**
        * Creates all sodecl::platform objects
        *
        * @todo needs a better description
        */
        void create_platforms() {
            cl_platform_id *cpPlatform = new cl_platform_id[m_platform_count];

            // get all platforms
            cl_int err = clGetPlatformIDs(m_platform_count, cpPlatform, NULL);
            if (err != CL_SUCCESS) {
                delete[] cpPlatform;
            }

            for (cl_uint i = 0; i < m_platform_count; i++) {
                m_platforms.push_back(new platform(cpPlatform[i]));
            }
            delete[] cpPlatform;
        }

        /**
        * Finds and returns the number of OpenCL platforms available.
        *
        * @return	The number of OpenCL platforms available. Returns -1 if the operation failed
        */
        int get_platform_count() {
            cl_uint platform_count;

            // get platform count
            cl_int err = clGetPlatformIDs(platform_count, NULL, &platform_count);

            if (err != CL_SUCCESS) {
                return -1;
            }

            return platform_count;
        }

        /**
        * Sets the sodecl object to use the CPU to integrate the ODE model.
        *
        * @todo	There could be an issue when selecting the OpenCL platform if there are more than one
        */
        void get_CPU_device() {

        }

        /******************************************************************************************
        SOFTWARE SECTION
        */

        /**
        * Create OpenCL context for the selected OpenCL platform and OpenCL device.
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int create_context() {
            cl_int err;
            cl_context context = clCreateContext(NULL, 1,
                                                 &(m_platforms[m_selected_platform]->m_devices[m_selected_device]->m_device_id),
                                                 NULL, NULL, &err);
            if (err != (cl_int) CL_SUCCESS) {
                std::cout << "Error: Failed to create context! " << err << std::endl;
                return 0;
            }
            m_contexts.push_back(context);

            //std::cout << "Size of m_contexts : " << m_contexts.size() << std::endl;
            return 1;
        }

        /**
        * Create OpenCL command queue for the selected OpenCL platform and OpenCL device.
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int create_command_queue() {
            cl_context context = m_contexts[0];

            cl_device_id device = m_platforms[m_selected_platform]->m_devices[m_selected_device]->m_device_id;

            cl_int err;
            cl_command_queue commands = clCreateCommandQueue(context, device, 0 , &err);
            if (err != CL_SUCCESS) {
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

        /**
        * Reads an OpenCL function or kernel
        *
        * @param	filename	OpenCL file
        */
        void read_kernel_file(char *filename) {
            std::ifstream t(filename);
            if (t.is_open()) {
                t.seekg(0, std::ios::end);
                size_t size = t.tellg();
                std::string str(size, ' ');
                t.seekg(0);
                t.read(&str[0], size);
                t.close();

                m_source_size = str.length();

                for (int i = 0; i < m_source_size; i++) {
                    m_kernel_sources.push_back(str[i]);
                }
            } else {
                // show message:
                std::cout << "Error opening file";
            }
        }

        /**
        * Adds the string str to the vector which stores the kernel string (i.e. OpenCL kernel code)
        *
        * @param	str		The sting to be pushed in the OpenCL kernel code vector
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull
        */
        int add_string_to_kernel_sources(string str) {
            // TODO: Add code to catch an exception in case the operation fails
            for (size_t i = 0; i < str.length(); i++) {
                m_kernel_sources.push_back(str[i]);
            }

            return 1;
        }

        /**
        * Convert a double to string.
        *
        * @param	x		Double number which will be coverted to string
        *
        * @return	A string of the converted double number
        */
        string double2string(double x) {
            // @todo Add code to catch an exception in the unlikely case of the conversion fails
            std::string s;
            {
                std::ostringstream ss;
                ss << x;
                s = ss.str();
            }
            return s;
        }

        /**
        * Form the OpenCL kernel string.
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int create_kernel_string() {
            // Create the parameters section of the kernel string. These parameter values are defines

            if (m_num_noi > 0) {
                add_string_to_kernel_sources("#define R123_USE_U01_DOUBLE 1");
                add_string_to_kernel_sources("\n");

                add_string_to_kernel_sources("#include <Random123/threefry.h>");
                add_string_to_kernel_sources("\n");

                add_string_to_kernel_sources("#include <Random123/u01.h>");
                add_string_to_kernel_sources("\n");
            }
            // Number of noise parameters
            add_string_to_kernel_sources("#define _numnoi_ ");
            add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_num_noi)));
            add_string_to_kernel_sources("\n");

            // List size
            add_string_to_kernel_sources("#define _listsize_ ");
            add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_list_size)));

            add_string_to_kernel_sources("\n");
            // Kernel steps
            add_string_to_kernel_sources("#define _numsteps_ ");
            add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_kernel_steps)));
            add_string_to_kernel_sources("\n");

            //cout << "Kernel steps done" << endl;

            // SODE solver time size
            add_string_to_kernel_sources("#define _m_dt_ ");
            //add_string_to_kernel_sources(std::to_string(static_cast<long double>(m_dt)));
            add_string_to_kernel_sources(double2string(m_dt));
            add_string_to_kernel_sources("\n");

            //cout << "SODE solver time size" << endl;

            // SODE system number of equations
            add_string_to_kernel_sources("#define _numeq_ ");
            add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_num_equat)));
            add_string_to_kernel_sources("\n");

            // SODE system number of parameters
            add_string_to_kernel_sources("#define _numpar_ ");
            add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_num_params)));
            add_string_to_kernel_sources("\n");

            if (m_solver == solver_Type::ImplicitEuler || m_solver == solver_Type::ImplicitMidpoint) {
                double epsilon1 = 1e-7;
                // Implicit StochasticEuler Newton-Raphson epsilon1 value
                add_string_to_kernel_sources("#define _epsilon1_ ");
                add_string_to_kernel_sources(double2string(epsilon1));
                add_string_to_kernel_sources("\n");
            }

            // Read the SODE system functions
            read_kernel_file(m_sode_system_string);
            add_string_to_kernel_sources("\n");

            std::vector<char> kernelpath_char;
            string kernelpath = m_kernel_path_str;

            // Choose the solver.
            switch (m_solver) {
                case solver_Type::StochasticEuler:
                    kernelpath.append("/stochastic_euler.cl");    // Stochastic Euler
                    break;
                case solver_Type::Euler:
                    kernelpath.append("/euler.cl");    // Euler
                    break;
                case solver_Type::RungeKutta:
                    kernelpath.append("/rk4.cl");    // Runge-Kutta
                    break;
                case solver_Type::ImplicitEuler:
                    kernelpath.append("/ie.cl");    // Implicit Euler
                    break;
                case solver_Type::ImplicitMidpoint:
                    kernelpath.append("/im.cl");    // Implicit midpoint
                    break;
                default:
                    std::cout << "No valid solver chosen!" << std::endl;
            }

            for (int i = 0; i < kernelpath.size(); i++) {
                kernelpath_char.push_back(kernelpath[i]);
            }
            kernelpath_char.push_back('\0');

            m_log->write(&*kernelpath_char.begin());
            m_log->write("\n");

            kernelpath_char.shrink_to_fit();

            read_kernel_file(&*kernelpath_char.begin());

            add_string_to_kernel_sources("\n");

            // Read the solver
            string kernelsolverpath_char = m_kernel_path_str;

            // Choose the solver.
            switch (m_solver) {
                case solver_Type::StochasticEuler:
                    kernelsolverpath_char.append("/stochastic_solver_caller.cl");
                    break;
                case solver_Type::Euler:
                    kernelsolverpath_char.append("/solver_caller.cl");
                    break;
                case solver_Type::RungeKutta:
                    kernelsolverpath_char.append("/solver_caller.cl");
                    break;
                case solver_Type::ImplicitEuler:
                    kernelsolverpath_char.append("/solver_caller.cl");
                    break;
                case solver_Type::ImplicitMidpoint:
                    kernelsolverpath_char.append("/solver_caller.cl");
                    break;
                default:
                    std::cout << "No valid solver chosen!" << std::endl;
            }
            //std::cout << kernelsolverpath_char << std::endl;
            read_kernel_file(&kernelsolverpath_char[0]);
            add_string_to_kernel_sources("\n");

            // Print the string
            //cout << m_kernel_sources.data() << endl;

            m_kernel_sources.shrink_to_fit();
            m_source_size = m_kernel_sources.size();

            //cout << m_kernel_sources.data() << endl;

            // This is for debug purpose
            std::ofstream out("Generated_kernel.cl");
            out << m_kernel_sources.data();
            out.close();

            return 1;
        }

        /**
        * Create OpenCL program for the selected OpenCL platform, OpenCL device and created OpenCL context.
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int create_program() {
            const char *srcptr[] = {m_kernel_sources.data()};

            //std::cout << "Size of string is: " << m_source_size << std::endl;

            //cout << "Size of kernel " << m_source_size << endl;

            cl_int err;
            cl_program program = clCreateProgramWithSource(m_contexts.at(0), 1, srcptr, (const size_t *) &m_source_size,
                                                           &err);
            if (err != CL_SUCCESS) {
                return 0;
            }

            // Create OpenCL Program
            m_programs.push_back(program);

            return 1;
        }

        /**
        * Adds a selected OpenCL build option in the vector which stores these OpenCL build options.
        *
        * @param	build_option	build_Option which will be stored in the vector
        *
        * @todo		This function should be expanded to perform all the operations for adding the OpenCL options in the char vector
        */
        void add_option_to_build_options(build_Option build_option) {
            m_build_options.push_back(build_option);
        }

        /**
        * Adds the characters of a string in the vector which stores the OpenCL build options.
        *
        * @param	str		String with the OpenCL build option
        */
        void add_string_to_build_options_str(string str) {
            for (size_t i = 0; i < str.length(); i++) {
                m_build_options_str.push_back(str[i]);
            }
        }

        /**
        * Builds OpenCL program for the selected OpenCL device.
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int build_program() {
            cl_device_id device_id = m_platforms[m_selected_platform]->m_devices[m_selected_device]->m_device_id;

            add_string_to_build_options_str("-I. ");

            for (build_Option option : m_build_options) {
                switch (option) {
                    case build_Option::FastRelaxedMath:
                        add_string_to_build_options_str("-cl-fast-relaxed-math ");
                        break;
                    case build_Option::stdCL20:
                        add_string_to_build_options_str("-cl-std=CL2.0 ");
                        break;
                    case build_Option::stdCL21:
                        add_string_to_build_options_str("-cl-std=CL2.1 ");
                        break;
                }
            }

            const char *options = &m_build_options_str[0];
            //cerr << "Build options string: " << options << endl;
            m_log->write("Build options string: ");
            m_log->write(options);
            m_log->write("\n");

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
            //const char * options = "-D KHR_DP_EXTENSION -cl-fast-relaxed-math";
            //const char * options = "";
            //const char * options = "-cl-fast-relaxed-math -cl-std=CL2.0";

            cl_int err = clBuildProgram(m_programs[0], 1, &device_id, options, NULL, NULL);
            if (err != CL_SUCCESS) {
                size_t len;
                char buffer[2048];

                std::cout << "Error: " << err << std::endl;

                std::cout << "Error: Failed to build program executable!" << std::endl;

                clGetProgramBuildInfo(m_programs[0], device_id, CL_PROGRAM_BUILD_LOG,
                                      sizeof(buffer), buffer, &len);

                std::cout << buffer << std::endl;
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
        int create_kernel(string kernelname, cl_program program) {
            cl_int err;

            cl_kernel kernel = clCreateKernel(program, kernelname.c_str(), &err);
            if (!kernel || err != CL_SUCCESS) {
                cerr << "Error: Failed to create compute kernel!" << std::endl;
                return 0;
            }

            m_kernels.push_back(kernel);

            return 1;
        }

        /**
        * Builds OpenCL program for the selected OpenCL device.
        *
        * @param	context		The context is the cl_context
        * @param	list_size	The list_size is the size of the buffers
        * @param	equat_num	The equat_num is number of equations
        * @param	param_num	The param_num is the number of parameters
        */
        int create_buffers(cl_context context, int list_size, int equat_num, int param_num) {
            cl_int errcode;
            // Create OpenCL device memory buffers
            m_mem_t0 = clCreateBuffer(context, CL_MEM_READ_WRITE, list_size * sizeof(cl_double), NULL, &errcode);
            if (errcode != CL_SUCCESS) {
                return 0;
            }

            m_mem_y0 = clCreateBuffer(context, CL_MEM_READ_WRITE, list_size * sizeof(cl_double) * equat_num, NULL,
                                      &errcode);
            if (errcode != CL_SUCCESS) {
                return 0;
            }

            m_mem_detterm = clCreateBuffer(context, CL_MEM_READ_WRITE, list_size * sizeof(cl_double) * equat_num, NULL,
                                           &errcode);
            if (errcode != CL_SUCCESS) {
                return 0;
            }

            m_mem_params = clCreateBuffer(context, CL_MEM_READ_ONLY, list_size * sizeof(cl_double) * param_num, NULL,
                                          &errcode);
            if (errcode != CL_SUCCESS) {
                return 0;
            }

            if (m_num_noi > 0) {
                m_mem_stoch = clCreateBuffer(context, CL_MEM_READ_WRITE, list_size * sizeof(cl_double) * equat_num,
                                             NULL, &errcode);
                if (errcode != CL_SUCCESS) {
                    return 0;
                }

                m_mem_rcounter = clCreateBuffer(context, CL_MEM_READ_ONLY, list_size * sizeof(cl_double), NULL,
                                                &errcode);
                if (errcode != CL_SUCCESS) {
                    return 0;
                }

                m_mem_noise = clCreateBuffer(context, CL_MEM_READ_ONLY, list_size * sizeof(cl_double) * m_num_noi, NULL,
                                             &errcode);
                if (errcode != CL_SUCCESS) {
                    return 0;
                }
            }

            return 1;
        }

        /**
        * Create OpenCL device memory buffer for the dt.
        *
        * @param	context		OpenCL context
        * @param	list_size	Number of orbits which the ODE or SDE system will be integrated for
        *
        * @return	param_num	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int create_dt_buffer(cl_context context, int list_size) {
            cl_int errcode;
            m_mem_dt = clCreateBuffer(context, CL_MEM_READ_WRITE, list_size * sizeof(cl_double), NULL, &errcode);
            if (errcode != CL_SUCCESS) {
                return 0;
            }

            return 1;
        }

        /**
        * Write data in the OpenCL memory buffers.
        *
        * @param	commands	OpenCL command queue
        * @param	list_size	Number of data points to be written in the OpenCL memory buffers
        * @param	equat_num	Number of equations of the ODE or SDE system
        * @param	param_num	Number of parameters of the ODE or SDE system
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int write_buffers(cl_command_queue commands, int list_size, int equat_num, int param_num) {
            int err = 0;
            err |= clEnqueueWriteBuffer(commands, m_mem_t0, CL_TRUE, 0, list_size * sizeof(cl_double), m_t0, 0, NULL,
                                        NULL);

            cl_double *temp = new cl_double[m_list_size * m_num_equat];
            for (int orbit = 0; orbit < list_size; orbit++) {
                int k = orbit * m_num_equat;
                for (int ieq = 0; ieq < m_num_equat; ieq++) {
                    int i = k + ieq;
                    temp[i] = m_y0[orbit * equat_num + ieq];
                }
            }

            err |= clEnqueueWriteBuffer(commands, m_mem_y0, CL_TRUE, 0, list_size * sizeof(cl_double) * equat_num, temp,
                                        0, NULL, NULL);

            delete[] temp;

            err |= clEnqueueWriteBuffer(commands, m_mem_params, CL_TRUE, 0, list_size * sizeof(cl_double) * param_num,
                                        m_params, 0, NULL, NULL);

            if (m_num_noi > 0) {
                err |= clEnqueueWriteBuffer(commands, m_mem_rcounter, CL_TRUE, 0, list_size * sizeof(cl_double),
                                            m_rcounter, 0, NULL, NULL);
            }

            if (err != CL_SUCCESS) {
                std::cout << "Error: Failed to write to source array!" << std::endl;
                return 0;
            }

            return 1;
        }

        /**
        * Write the initial ODE solver time steps for each ODE or SDE system orbit.
        *
        * @param	commands	OpenCL command queue
        * @param	list_size	Number of data points to be written in the OpenCL memory buffers
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int write_dt_buffer(cl_command_queue commands, int list_size) {
            int err = 0;
            err |= clEnqueueWriteBuffer(commands, m_mem_dt, CL_TRUE, 0, list_size * sizeof(cl_double), m_dts, 0, NULL,
                                        NULL);

            if (err != CL_SUCCESS) {
                std::cout << "Error: Failed to write to source array!" << std::endl;
                return 0;
            }

            return 1;
        }

        /**
        * @todo what is this?
        */
        void input_data() {

        }

        /**
        * Sets OpenCL kernel arguments.
        *
        * @param	commands	OpenCL command queue
        * @param	list_size	Number of data points to be written in the OpenCL memory buffers
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int set_kernel_args(cl_kernel kernel) {
            // Set the arguments to the compute kernel
            cl_int err = 0;
            err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &m_mem_t0);
            err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &m_mem_y0);
            //err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &m_mem_detterm);
            err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &m_mem_params);

            if (m_num_noi > 0) {
                //err |= clSetKernelArg(kernel, 4, sizeof(cl_mem), &m_mem_stoch);
                err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &m_mem_rcounter);
                //err |= clSetKernelArg(kernel, 6, sizeof(cl_mem), &m_mem_noise);
            }

            if (err != CL_SUCCESS) {
                std::cout << "Error: Failed to set kernel arguments! " << err << std::endl;
                return 0;
            }
            return 1;
        }

    public:

        /**
        * Sets the initial value for the time variable.
        *
        * @param	t0		Initial time value for each orbit to be calculated
        */
        void set_t0(cl_double *t0) {
            m_t0 = t0;
        }

        /**
        * Sets the initial values for the variables of the ODE or SDE system.
        *
        * @param	y0		Initial variables values for each orbit to be calculated
        */
        void set_y0(cl_double *y0) {
            m_y0 = y0;
        }

        /**
        * Sets the parameter values for the variables of the ODE or SDE system.
        *
        * @param	params		Parameter variables values for each orbit to be calculated
        */
        void set_params(cl_double *params) {
            m_params = params;
        }

        /**
        * Reads data from a binary file and saves the data in an array.
        *
        * @param	filename	Filename to read the data from
        * @param	data		Pointer to cl_double array to store the read data to
        *
        * @param	params		Returns 1 if the read of the data is successful or 0 if unsuccessful
        */
        int read_binary_data_from_file(char *filename, cl_double *data) {
            ifstream ifs;
            ifs.open(filename, ios::in | ios::binary | ios::ate);

            if (ifs.good()) {
                streampos size = ifs.tellg();

                ifs.seekg(0, ios::beg);

                vector<double> vectordata;
                vectordata.resize(size);
                ifs.read(reinterpret_cast<char *>(vectordata.data()), size);

                for (int i = 0; i < size / 8; i++) {
                    data[i] = vectordata.at(i);
                }

                ifs.close();
                return 1;
            } else {
                cout << "ERROR: can't open " << filename << " file." << endl;
                return 0;
            }

            return 1;
        }

        /**
        * Reads data from a txt file and saves the data in an array.
        *
        * @param	filename	Filename to read the data from
        * @param	data		Pointer to cl_double array to store the read data to
        *
        * @param	params		Returns 1 if the read of the data is successful or 0 if unsuccessful
        */
        int read_data_from_file(char *filename, cl_double *data) {
            // a string to store line of text
            string textLine;
            // try to open a file
            ifstream ifs(filename, ifstream::in);

            int j = 0;
            if (ifs.good()) {
                while (!ifs.eof()) {
                    // read line of text
                    getline(ifs, textLine);

                    if (!textLine.empty()) {
                        istringstream iss(textLine);

                        vector <string> strs;
                        copy(istream_iterator<string>(iss),
                             istream_iterator<string>(),
                             back_inserter(strs));

                        for (std::vector<string>::size_type i = 0; i != strs.size(); i++) {
                            data[j] = atof(strs[i].c_str());
                            j++;
                        }
                    }
                }
                ifs.close();
                return 1;
            } else {
                cout << "ERROR: can't open " << filename << " file." << endl;
                return 0;
            }

            return 1;
        }

        /**
        * Executes the ODE or SDE solver on the selected OpenCL device.
        *
        * @param	params		Returns 1 if the read of the data is successful or 0 if unsuccessful
        */
        int run_sode_solver() {
            // Output binary files
            std::ofstream output_stream;
            if (m_output_type == sodecl::output_Type::File) {
                output_stream.open(m_outputfile_str, std::ios::binary | std::ios::app | std::ios::out);
            }

            cl_double *t_out = new cl_double[m_list_size];
            cl_double *orbits_out = new cl_double[m_list_size * m_num_equat];

            size_t global = size_t(m_list_size);
            size_t local;

            local = m_local_group_size;
            if (m_local_group_size > m_list_size) {
                local = m_local_group_size;
            }

            //cout << "The local group size is: " << local << endl;
            m_log->write("The local group size is: ");
            m_log->write((double) local);
            m_log->write("\n");

            timer start_timer;

            // Run the initial values to the output file.
            //std::cout << "Running kernel.." << std::endl;
            cl_int err;
            for (int j = 0; j < (m_int_time / (m_dt * m_kernel_steps)); j++) {
                //std::cout << "Running kernel.." << std::endl;
                //// Read buffer g into a local list
                ////err = clEnqueueReadBuffer(m_command_queues[0], m_mem_t0, CL_TRUE, 0, m_list_size * sizeof(cl_double), t_out, 0, NULL, NULL);
                err = clEnqueueReadBuffer(m_command_queues[0], m_mem_y0, CL_TRUE, 0,
                                          m_list_size * sizeof(cl_double) * m_num_equat, orbits_out, 0, NULL, NULL);

                try {
                    if (local != 0) {
                        err = clEnqueueNDRangeKernel(m_command_queues[0], m_kernels[0], 1, NULL, &global, &local, 0,
                                                     NULL, NULL);
                    } else {
                        err = clEnqueueNDRangeKernel(m_command_queues[0], m_kernels[0], 1, NULL, &global, NULL, 0, NULL,
                                                     NULL);
                    }
                }
                catch (const std::exception &e) {
                    m_log->write("The call to the OpenCL device has failed.");
                    m_log->write(e.what());
                }

                if (err) {
                    //cerr << "Error: Failed to execute kernel!" << std::endl;
                    m_log->write("Error: Failed to execute kernel!");
                    return 0;
                }
                clFlush(m_command_queues[0]);
                //clFinish(m_command_queues[0]);

                // Save data to disk or to data array - all variables
                for (int jo = 0; jo < m_num_equat; jo++) {
                    int e = m_outputPattern[jo];
                    if (e > 0) {
                        for (int ji = jo; ji < m_list_size * m_num_equat; ji = ji + m_num_equat) {
                            switch (m_output_type) {
                                case sodecl::output_Type::Array :
                                    m_output.push_back(orbits_out[ji]);
                                    break;
                                case sodecl::output_Type::File :
                                    output_stream.write((char *) (&orbits_out[ji]), sizeof(cl_double));
                                    break;
                                case sodecl::output_Type::None :
                                    break;
                            }
                        }
                    }
                }
            }

            // Save the data from the last kernel call.
            err = clEnqueueReadBuffer(m_command_queues[0], m_mem_y0, CL_TRUE, 0,
                                      m_list_size * sizeof(cl_double) * m_num_equat, orbits_out, 0, NULL, NULL);
            // Save data to disk or to data array - all variables
            for (int jo = 0; jo < m_num_equat; jo++) {
                int e = m_outputPattern[jo];
                if (e > 0) {
                    for (int ji = jo; ji < m_list_size * m_num_equat; ji = ji + m_num_equat) {
                        //m_output.push_back(orbits_out[ji]);
                        switch (m_output_type) {
                            case sodecl::output_Type::Array :
                                m_output.push_back(orbits_out[ji]);
                                break;
                            case sodecl::output_Type::File :
                                output_stream.write((char *) (&orbits_out[ji]), sizeof(cl_double));
                                break;
                            case sodecl::output_Type::None :
                                break;
                        }
                    }
                }
            }

            //m_output = output_data;

            double walltime = start_timer.stop_timer();
            std::cout << "Compute results runtime: " << walltime << " sec.\n";
            m_log->write("Compute results runtime: ");
            m_log->write(walltime);
            m_log->write("sec.\n");

            if (m_output_type == sodecl::output_Type::File) {
                output_stream.close();
            }

            m_log->toFile();

            delete[] t_out;
            delete[] orbits_out;

            return 1;
        }

        /**
        * Setups the selected ODE or SDE solver OpenCL kernel source.
        *
        * @return 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int setup_sode_solver() {
            // To create a cl string with the program to run
            if (create_kernel_string() == 0) {
                std::cout << "Kernel code creation failed." << std::endl;
                return 0;
            }
            //m_log->write("Kernel code creation successed.\n");

            if (create_context() == 0) {
                std::cout << "Context creation failed." << std::endl;
                return 0;
            }
            //m_log->write("Context created.\n");

            if (create_program() == 0) {
                std::cout << "Failed to create OpenCL program from source." << std::endl;
                return 0;
            }

            if (build_program() == 0) {
                std::cout << "Failed to build kernel." << std::endl;
                return 0;
            }

            if (create_kernel("solver_caller", m_programs[0]) == 0) {
                std::cout << "Failed to create kernel." << std::endl;
                return 0;
            }

            if (create_command_queue() == 0) {
                std::cout << "Failed to create command queue." << std::endl;
                return 0;
            }

            if (create_buffers(m_contexts[0], m_list_size, m_num_equat, m_num_params) == 0) {
                std::cout << "Failed to create the OpenCL buffers." << std::endl;
                return 0;
            }

            if (write_buffers(m_command_queues[0], m_list_size, m_num_equat, m_num_params) == 0) {
                std::cout << "Failed to write the data to the OpenCL buffers." << std::endl;
                return 0;
            }

            // assign the inputs to kernel
            if (set_kernel_args(m_kernels[0]) == 0) {
                std::cout << "Failed to set kernel arguments." << std::endl;
                return 0;
            }

            //m_log->toFile();

            return 1;
        }
    };

}


#endif // sodecl_system_HPP
