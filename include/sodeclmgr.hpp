//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef SODECL_system_HPP
#define SODECL_system_HPP

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <random>

#include "sodecl_export.h"
#include "CL/cl.h"
#include "platform.hpp"

using namespace std;

namespace sodecl {
    /**
    *  Manager class of sodecl.
    */
    class SODECL_EXPORT sodeclmgr {
        /*********************************************************************************************
        VARIABLES SECTION
        */
    public:

        std::vector<double> m_output;         /**< Integration output */
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

        std::vector<cl_context> m_contexts;                /**< OpenCL command contexts vector */
        std::vector<cl_command_queue> m_command_queues;        /**< OpenCL command queues vector */
        std::vector<char> m_kernel_sources;        /**< Char vector which stores the OpenCL kernel source string. @todo store multiple kernel source strings */
        std::string m_build_options_str;    /**< Char vector which stores the OpenCL build options string */
        std::vector<build_Option> m_build_options;        /**< build_Option vector which stores the OpenCL build options selection */
        char *m_source_str;            /**< OpenCL kernel string */
        string m_kernel_path_str;        /**< OpenCL kernels solvers path */
        size_t m_source_size;            /**< OpenCL kernel string size */
        std::vector<cl_program> m_programs;                /**< OpenCL programs vector */
        std::vector<cl_kernel> m_kernels;                /**< OpenCL kernels vector */
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
                  output_Type output_type);

        /**
        *  Destructor. Deletes all objects, arrays, pointers, etc.
        */
        ~sodeclmgr();

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
        int choose_device(cl_uint platform_num, device_Type device_type, cl_uint device_num);

        /**
        *  Sets the number of work items in each group.
        *
        * @param	local_group_size	Number of work items for each group
        */
        void set_local_group_size(int local_group_size);

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
        void create_platforms();

        /**
        * Finds and returns the number of OpenCL platforms available.
        *
        * @return	The number of OpenCL platforms available. Returns -1 if the operation failed
        */
        int get_platform_count();

        /******************************************************************************************
        SOFTWARE SECTION
        */

        /**
        * Create OpenCL context for the selected OpenCL platform and OpenCL device.
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int create_context();

        /**
        * Create OpenCL command queue for the selected OpenCL platform and OpenCL device.
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int create_command_queue();

        /******************************************************************************************
        ODE SOLVERS SECTION
        */

    private:

        /**
        * Reads an OpenCL function or kernel
        *
        * @param	filename	OpenCL file
        */
        void read_kernel_file(char *filename);

        /**
        * Adds the string str to the vector which stores the kernel string (i.e. OpenCL kernel code)
        *
        * @param	str		The sting to be pushed in the OpenCL kernel code vector
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull
        */
        int add_string_to_kernel_sources(string str);

        /**
        * Convert a double to string.
        *
        * @param	x		Double number which will be coverted to string
        *
        * @return	A string of the converted double number
        */
        string double2string(double x);

        /**
        * Form the OpenCL kernel string.
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int create_kernel_string();

        /**
        * Create OpenCL program for the selected OpenCL platform, OpenCL device and created OpenCL context.
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int create_program();

        /**
        * Adds a selected OpenCL build option in the vector which stores these OpenCL build options.
        *
        * @param	build_option	build_Option which will be stored in the vector
        *
        * @todo		This function should be expanded to perform all the operations for adding the OpenCL options in the char vector
        */
        void add_option_to_build_options(build_Option build_option);

        /**
        * Adds the characters of a string in the vector which stores the OpenCL build options.
        *
        * @param	str		String with the OpenCL build option
        */
        void add_string_to_build_options_str(string str);

        /**
        * Builds OpenCL program for the selected OpenCL device.
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int build_program();

        /// <summary>
        /// Create the OpenCL kernel.
        /// </summary>
        /// <param name="kernelname">Name of the OpenCL kernel.</param>
        /// <param name="program">OpenCL program.</param>
        /// <returns>Returns 1 if the operations were succcessfull or 0 if they were unsuccessfull.</returns>
        int create_kernel(string kernelname, cl_program program);

        /**
        * Builds OpenCL program for the selected OpenCL device.
        *
        * @param	context		The context is the cl_context
        * @param	list_size	The list_size is the size of the buffers
        * @param	equat_num	The equat_num is number of equations
        * @param	param_num	The param_num is the number of parameters
        */
        int create_buffers(cl_context context, int list_size, int equat_num, int param_num);

        /**
        * Create OpenCL device memory buffer for the dt.
        *
        * @param	context		OpenCL context
        * @param	list_size	Number of orbits which the ODE or SDE system will be integrated for
        *
        * @return	param_num	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int create_dt_buffer(cl_context context, int list_size);

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
        int write_buffers(cl_command_queue commands, int list_size, int equat_num, int param_num);

        /**
        * Sets OpenCL kernel arguments.
        *
        * @param	commands	OpenCL command queue
        * @param	list_size	Number of data points to be written in the OpenCL memory buffers
        *
        * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int set_kernel_args(cl_kernel kernel);

    public:

        /**
        * Sets the initial value for the time variable.
        *
        * @param	t0		Initial time value for each orbit to be calculated
        */
        void set_t0(cl_double *t0);

        /**
        * Sets the initial values for the variables of the ODE or SDE system.
        *
        * @param	y0		Initial variables values for each orbit to be calculated
        */
        void set_y0(cl_double *y0);

        /**
        * Sets the parameter values for the variables of the ODE or SDE system.
        *
        * @param	params		Parameter variables values for each orbit to be calculated
        */
        void set_params(cl_double *params);

        /**
        * Reads data from a binary file and saves the data in an array.
        *
        * @param	filename	Filename to read the data from
        * @param	data		Pointer to cl_double array to store the read data to
        *
        * @param	params		Returns 1 if the read of the data is successful or 0 if unsuccessful
        */
        int read_binary_data_from_file(char *filename, cl_double *data);

        /**
        * Reads data from a txt file and saves the data in an array.
        *
        * @param	filename	Filename to read the data from
        * @param	data		Pointer to cl_double array to store the read data to
        *
        * @param	params		Returns 1 if the read of the data is successful or 0 if unsuccessful
        */
        int read_data_from_file(char *filename, cl_double *data);

        /**
        * Executes the ODE or SDE solver on the selected OpenCL device.
        *
        * @param	params		Returns 1 if the read of the data is successful or 0 if unsuccessful
        */
        int run_sode_solver();

        /**
        * Setups the selected ODE or SDE solver OpenCL kernel source.
        *
        * @return 1 if the operations were succcessfull or 0 if they were unsuccessful
        */
        int setup_sode_solver();
    };

}


#endif // SODECL_system_HPP
