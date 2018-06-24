//---------------------------------------------------------------------------//
// Copyright (c) 2015,2018 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef SODECL_EULER_HPP
#define SODECL_EULER_HPP

#include "sodecl.hpp"
#include <ctime>
#include <limits>

namespace sodecl
{

/**
 * @brief Class that implements the functions needed for the Euler solver.
 * 
 */
class stochastic_euler : public solver_interface
{
  private:

    cl_mem m_mem_noise;         /**< OpenCL memory buffer which stores the random numbers */
    cl_mem m_mem_rcounter;      /**< OpenCL memory buffer which stores the counters for each workitem for the random number generation */

    opencl_source_stochastic_noise* m_opencl_source_stochastic_noise;
    opencl_source_stochastic_euler* m_opencl_source_stochastic_euler;

    string m_kernel_path_str;
    double m_dt;

    int m_num_noi;

	cl_double*	m_rcounter;		/**< Counter that counts the number of calls to the random number generator */ 


  public:
    /**
     * @brief Constructor for the Euler solver. 
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
     */
    stochastic_euler(
          sodecl::opencl_mgr*   ocl_mgr,
          string        kernel_path_str,
          char*         ode_system_str,
          double        dt,
          int           int_time,
          int           kernel_steps,
          int           num_equat,
          int           num_params,
          int           num_noi,
          int           list_size,
          cl_double*    y0,
          cl_double*    params) : solver_interface( ocl_mgr,
                                                    kernel_path_str,
                                                    ode_system_str,
                                                    dt,
                                                    int_time,
                                                    kernel_steps,
                                                    num_equat,
                                                    num_params,
                                                    list_size,
                                                    y0,
                                                    params)
    {
        m_opencl_source_stochastic_noise = new opencl_source_stochastic_noise(kernel_path_str, dt);

        m_opencl_source_stochastic_euler = new opencl_source_stochastic_euler(kernel_path_str, dt, ode_system_str, kernel_steps, num_equat, num_params, num_noi);

        m_num_noi = num_noi;

        m_kernel_path_str = kernel_path_str;
        m_dt = dt;

        std::mt19937_64 gen( time(0));
        m_rcounter = new double[m_list_size];
        for (int i = 0; i < m_list_size; i++)
        {
            m_rcounter[i] = gen();
        }
    }

    /**
     * @brief Virtual destructor.
     * 
     */
    ~stochastic_euler()
    {
        delete m_opencl_source_stochastic_noise;
        delete m_opencl_source_stochastic_euler;
    }

    /**
     * @brief Forms the OpenCL kernel string(s).
     * 
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    int create_kernel_strings()
    {        
        m_kernel_strings.push_back(m_opencl_source_stochastic_noise->create_kernel_string());
        m_kernel_strings.push_back(m_opencl_source_stochastic_euler->create_kernel_string());

        return 1;
    }

    /**
		* Builds OpenCL program for the selected OpenCL device.
		*
		* @param	context		The context is the cl_context
		* @param	norbits	    The norbits is the size of the buffers
		* @param	equat_num	The equat_num is number of equations
		* @param	param_num	The param_num is the number of parameters
		*/
    int create_buffers(cl_context context, int norbits, int equat_num, int param_num)
    {
        m_mem_t0 = m_opencl_mgr->create_buffer(context, norbits, CL_MEM_READ_WRITE);
        m_mem_y0 = m_opencl_mgr->create_buffer(context, norbits*equat_num, CL_MEM_READ_WRITE);
        m_mem_params = m_opencl_mgr->create_buffer(context, norbits*param_num, CL_MEM_READ_WRITE);
        m_mem_rcounter = m_opencl_mgr->create_buffer(context, norbits, CL_MEM_READ_WRITE);
        m_mem_noise = m_opencl_mgr->create_buffer(context, m_num_noi * m_kernel_steps * norbits, CL_MEM_READ_WRITE);

        return 0;
    }

    /**
     * @brief Writes data in the OpenCL memory buffers.
     * 
     * @param	commands	OpenCL command queue.
     * @param	list_size	Number of data points to be written in the OpenCL memory buffers.
     * @param	equat_num	Number of equations of the ODE or SDE system.
     * @param	param_num	Number of parameters of the ODE or SDE system.
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    int write_buffers(cl_command_queue commands, int list_size, int equat_num, int param_num)
    {
        int err = 0;

        err |= clEnqueueWriteBuffer(commands, m_mem_y0, CL_TRUE, 0, list_size * equat_num * sizeof(cl_double), m_y0, 0, NULL, NULL);

        err |= clEnqueueWriteBuffer(commands, m_mem_params, CL_TRUE, 0, list_size * param_num * sizeof(cl_double), m_params, 0, NULL, NULL);

        if (err != CL_SUCCESS)
        {
            std::cout << "Error: Failed to write to source array!" << std::endl;
            return 0;
        }

        return 1;
    }

    /**
     * @brief Writes data in the OpenCL memory buffers.
     * 
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    int write_buffer_noise()
    {
        int err = 0;
        err = m_opencl_mgr->write_to_buffer(m_mem_rcounter, m_rcounter, m_list_size);
        return err;
    }

    /**
    * Sets OpenCL kernel arguments.
    *
    * @param	commands	OpenCL command queue
    * @param	list_size	Number of data points to be written in the OpenCL memory buffers
    *
    * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
    */
    int set_kernel_args_noise(cl_kernel kernel)
    {
        // Set the arguments to the compute kernel
        int err = 0;
        err += m_opencl_mgr->set_kernel_arg(kernel, m_mem_noise, 0);
        err += m_opencl_mgr->set_kernel_arg(kernel, m_mem_rcounter, 1);
        if (err > 0)
        {
            std::cout << "Error: Failed to set kernel arguments! " << err << std::endl;
            return 1;
        }
        return 0;
    }

    /**
    * Sets OpenCL kernel arguments.
    *
    * @param	commands	OpenCL command queue
    * @param	list_size	Number of data points to be written in the OpenCL memory buffers
    *
    * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
    */
    int set_kernel_args_solver(cl_kernel kernel)
    {
        int err = 0;
        err += m_opencl_mgr->set_kernel_arg(kernel, m_mem_t0, 0);
        err += m_opencl_mgr->set_kernel_arg(kernel, m_mem_y0, 1);
        err += m_opencl_mgr->set_kernel_arg(kernel, m_mem_params, 2);
        err += m_opencl_mgr->set_kernel_arg(kernel, m_mem_noise, 3);
        if (err > 0)
        {
            std::cout << "Error: Failed to set kernel arguments! " << err << std::endl;
            return 1;
        }
        return 0;
    }

    /**
     * @brief Setups the selected ODE solver OpenCL kernel source.
     * 
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    int setup_solver()
    {
        // Create the OpenCL source strings
        // For the stochastic euler this includes the noise numbers generation and the integration method

        cl_program temp = m_opencl_mgr->create_program(m_kernel_strings.at(0));
        if (temp==NULL)
        {
            std::cout << "FU" << std::endl;
        }
        m_programs.push_back(temp);
        
        temp = m_opencl_mgr->create_program(m_kernel_strings.at(1));
        if (temp==NULL)
        {
            std::cout << "FU" << std::endl;
        }
        m_programs.push_back(temp);

        // Build programs
        m_opencl_mgr->build_program(m_programs.at(0));
        m_opencl_mgr->build_program(m_programs.at(1));

        // Create the OpenCL buffers
        m_num_noi = 3;
        create_buffers(m_opencl_mgr->m_context, 10, 3, 3);

        write_buffer_noise();

        // Create the OpenCL kernels
        // One for the noise numbers generation and one for the integration method

        cl_kernel temp_kernel = m_opencl_mgr->create_kernel("random_numbers", m_programs.at(0));
        if (temp_kernel==NULL)
        {
            std::cout << "FU" << std::endl;
        }
        m_kernels.push_back(temp_kernel);

        temp_kernel = m_opencl_mgr->create_kernel("solver_caller", m_programs.at(1));
        if (temp_kernel==NULL)
        {
            std::cout << "FU" << std::endl;
        }
        m_kernels.push_back(temp_kernel);

        // Assign arguments to the kernels
        set_kernel_args_noise(m_kernels.at(0));
        set_kernel_args_solver(m_kernels.at(1));

		return 0;
    }

    /**
     * @brief Executes the ODE solver on the selected OpenCL device.
     * 
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    int run_solver()
    {
        size_t global_noise = size_t(m_list_size*m_kernel_steps*m_num_noi);
        size_t global_solver = size_t(m_list_size);
        size_t local;

        cl_int err;
        err = clEnqueueNDRangeKernel(m_opencl_mgr->m_command_queue, m_kernels.at(0), 1, NULL, &global_noise, NULL, 0, NULL, NULL);
		clFinish(m_opencl_mgr->m_command_queue);


        err = clEnqueueNDRangeKernel(m_opencl_mgr->m_command_queue, m_kernels.at(1), 1, NULL, &global_solver, NULL, 0, NULL, NULL);
		clFinish(m_opencl_mgr->m_command_queue);

        cl_double* orbits_out = new cl_double[m_num_noi * m_kernel_steps * m_list_size];
        err = clEnqueueReadBuffer(m_opencl_mgr->m_command_queue, m_mem_noise, CL_TRUE, 0, m_num_noi * m_kernel_steps * m_list_size * sizeof(cl_double), orbits_out, 0, NULL, NULL);
        clFinish(m_opencl_mgr->m_command_queue);

        for (int i=0; i<10; i++)
        {
            std::cout << orbits_out[i] << std::endl;
        }

        m_log->toFile();

        return 0;
    }
};
}

#endif // SODECL_EULER_HPP