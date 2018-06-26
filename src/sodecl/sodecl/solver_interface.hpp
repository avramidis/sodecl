//---------------------------------------------------------------------------//
// Copyright (c) 2015,2018 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef SODECL_SOLVER_INTERFACE_HPP
#define SODECL_SOLVER_INTERFACE_HPP

#include "sodecl.hpp"

namespace sodecl
{

/**
 * @brief Abstract class that acts as an interface for the implementation of different ODE or SDE solvers.
 * 
 */
class solver_interface
{
  public:

    /********************************************************************************************
    ODE SOLVER SECTION VARIABLES
    */

    char*           m_ode_system_string;    /**< ODE system OpenCL function */
    double          m_dt;                   /**< Solver (initial) time step in seconds */
    double          m_int_time;             /**< Time units to integrate the ODE system */
    int             m_kernel_steps;         /**< Number of step the solver with perform in each kernel call */
    int             m_num_dt_steps;         /**< Number of dt steps for the state of the ODE system to be saved */
    int             m_list_size;            /**< Number of ODE system parameter combinations */
    int             m_num_equat;            /**< Number of ODE system equations */
    int             m_num_params;           /**< Number of ODE system parameters */
    output_Type     m_output_type;          /**< Type of output (binary file or array) */
    int*            m_outputPattern;        /**< Array which holds the output pattern. E.g. if the system has 3 equations the array {1,2,3}   
                                                 will write to the output file the results of the 1st then 2nd and then 3rd state variable */

    // arrays with the state of the system for each parameter combination
    cl_double*	            m_t0;			  /**< Client memory buffer which stores the phase state of each integration output step */  
    cl_double*	            m_y0;			  /**< Client memory buffer which stores the phase state of each integration output step */  
    cl_double*	            m_params;		/**< Client memory buffer which stores the parameter values of each integration orbit of the ODE system */
    std::vector<cl_double>  m_output;	  /**< Integration output */ 
    
    /********************************************************************************************
    OPENCL SOFTWARE SECTION VARIABLES
    */
    opencl_mgr*                     m_opencl_mgr;           /**< Pointer to the object with the OpenCL operations */
    
    std::vector<string>             m_kernel_strings;       /**< Char vector which stores the OpenCL kernel source string. @todo store multiple kernel source strings */
    std::string                     m_build_options_str;    /**< Char vector which stores the OpenCL build options string */
    
    char*                           m_source_str;           /**< OpenCL kernel string */
    string                          m_kernel_path_str;      /**< OpenCL kernels solvers path */
    size_t                          m_source_size;          /**< OpenCL kernel string size */
    
    std::vector<cl_program>         m_programs;             /**< OpenCL programs vector */
    std::vector<cl_kernel>          m_kernels;              /**< OpenCL kernels vector */


    // OpenCL buffers
    cl_mem	                m_mem_t0;				    /**< OpenCL memory buffer which stores the time value of each integration output step */ 
    cl_mem	                m_mem_y0;				    /**< OpenCL memory buffer which stores the phase state of each integration output step */ 
    cl_mem	                m_mem_params;			  /**< OpenCL memory buffer which stores the parameter values of each integration orbit of the ODE system */ 
    cl_mem	                m_mem_dt;				    /**< OpenCL memory buffer which stores the time step for each integration orbit of the ODE system */ 
    char*	                  m_outputfile_str;		/**< Path to the results output file */

    // Log mechanisms
    clog*	m_log;				/**< Pointer for log */
    
    /**
     * @brief Default constructor.
     * 
     * @param kernel_path_str   Path to the SODECL OpenCL kernel source files.
     * @param ode_system_str    Path to the OpenCL ODE system source file.
     * @param dt                ODE solver time step size.
     * @param int_time          Length of time in seconds the the ODE system with be integrated for.
     * @param kernel_steps      Number of steps the ODE solver will perform in each OpenCL device call.
     * @param num_equat         Number of equations of the ODE system.
     * @param num_params        Number of parameters of the ODE system.
     * @param list_size         Number of orbits to be integrated for the ODE system
     * @param output_type       Specifies the location where the output of the integration of the ODE system will be stored.
     * @param y0                Client memory buffer which stores the phase state of each integration output step.
     * @param params            Client memory buffer which stores the parameter values of each integration orbit of the ODE system.
     */
    solver_interface(sodecl::opencl_mgr*  ocl_mgr,
                     string               kernel_path_str,
                     char*                ode_system_str,
                     double               dt,
                     double               int_time,
                     int                  kernel_steps,
                     int                  num_equat,
                     int                  num_params,
                     int                  list_size,
                     cl_double*           y0,
                     cl_double*           params)
    {
        // Initialise the clog object
		    m_log = clog::getInstance();

        m_opencl_mgr = ocl_mgr;

        // m_ode_system_string = ode_system_str;
        m_dt = dt;
        m_int_time = int_time;
        m_kernel_steps = kernel_steps;
        m_list_size = list_size;
        m_num_equat = num_equat;
        m_num_params = num_params;
        // m_output_type = output_type;

        m_t0 = new cl_double[m_list_size];
        for (int i = 0; i < m_list_size; i++)
        { 
            m_t0[i] = 0.0;
        }
        m_y0 = y0;
        m_params = params;

        m_outputPattern = new int[m_num_equat];
        for (int i = 0; i < m_num_equat; i++)
        {
          //m_outputPattern[i] = i+1;
          m_outputPattern[i] = 1;
        }
        m_outputPattern[0] = 1;

        // m_outputPattern = new int[m_num_equat];
        // for (int i = 0; i < m_num_equat; i++)
        // {
        //     //m_outputPattern[i] = i+1;
        //     m_outputPattern[i] = 1;
        // }
        // m_outputPattern[0] = 1;

        // // Find the host's platforms
        // // get the number of platforms in the system
        // m_platform_count = m_opencl_mgr->get_opencl_platform_count();
        // if (m_platform_count == -1)
        // {
        //     cerr << "Error getting OpenCL planform number!" << endl;
        // }

        // // create all sodecl::platform objects, one for each OpenCL platform
        // m_opencl_mgr->create_opencl_platforms();

        // // Add default OpenCL build options
        // m_build_options.push_back(build_Option::FastRelaxedMath);
        // //m_build_options.push_back(build_Option::stdCL20);

        // m_local_group_size = 0;
    }

    /**
     * @brief Virtual destructor.
     * 
     */
    virtual ~solver_interface(){};

    /**
     * @brief Forms the OpenCL kernel string(s).
     * 
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    virtual int create_kernel_strings() = 0;

    /**
     * @brief Creates the OpenCL buffers needed for the solver.
     * 
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    virtual int create_buffers() = 0;

    /**
     * @brief Writes data in the OpenCL memory buffers.
     * 
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    virtual int write_buffers() = 0;

    /**
     * Setups the selected ODE or SDE solver OpenCL kernel source.
     *
     * @return 1 if the operations were succcessfull or 0 if they were unsuccessful
     */
    /**
     * @brief Setups the selected ODE or SDE solver OpenCL kernel source.
     * 
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    virtual int setup_solver() = 0;

    /**
     * @brief Executes the ODE or SDE solver on the selected OpenCL device.
     * 
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    virtual int run_solver() = 0;

  protected:
  
};
}

#endif // SODECL_SOLVER_INTERFACE_HPP