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
          double        int_time,
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
        cl_int err_code;
        m_mem_t0 = clCreateBuffer(context, CL_MEM_READ_WRITE, norbits * sizeof(cl_double), NULL, &err_code);
        if (err_code != CL_SUCCESS)
        {
            std::cout << "Error: Failed to create OpenCL buffer 0!" << std::endl;
            return 0;
        }

        m_mem_y0 = clCreateBuffer(context, CL_MEM_READ_WRITE, norbits * sizeof(cl_double) * equat_num, NULL, &err_code);
        if (err_code != CL_SUCCESS)
        {
            std::cout << "Error: Failed to create OpenCL buffer 1!" << std::endl;
            return 0;
        }

        m_mem_params = clCreateBuffer(context, CL_MEM_READ_ONLY, norbits * sizeof(cl_double) * param_num, NULL, &err_code);
        if (err_code != CL_SUCCESS)
        {
            std::cout << "Error: Failed to create OpenCL buffer 2!" << std::endl;
            return 0;
        }

        m_mem_rcounter = clCreateBuffer(context, CL_MEM_READ_WRITE, norbits * sizeof(cl_double), NULL, &err_code);
        if (err_code != CL_SUCCESS)
        {
            std::cout << "Error: Failed to create OpenCL buffer 3!" << std::endl;
            return 0;
        }

        m_mem_noise = clCreateBuffer(context, CL_MEM_READ_WRITE, m_num_noi * m_kernel_steps * norbits * sizeof(cl_double), NULL, &err_code);
        if (err_code != CL_SUCCESS)
        {
            std::cout << "Error: Failed to create OpenCL buffer 4!" << std::endl;
            std::cout << err_code << std::endl;
            return 0;
        }

        return 1;
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

        cl_double *temp = new cl_double[m_list_size*m_num_equat];
        for (int orbit = 0; orbit < list_size; orbit++)
        {
            int k = orbit * m_num_equat;
            for (int ieq = 0; ieq < m_num_equat; ieq++)
            {
                int i = k + ieq;
                temp[i] = m_y0[orbit*equat_num + ieq];
            }
        }

        err |= clEnqueueWriteBuffer(commands, m_mem_y0, CL_TRUE, 0, list_size * sizeof(cl_double)*equat_num, temp, 0, NULL, NULL);

        delete[] temp;

        err |= clEnqueueWriteBuffer(commands, m_mem_params, CL_TRUE, 0, list_size * sizeof(cl_double)*param_num, m_params, 0, NULL, NULL);

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
    int write_noise_buffer()
    {
        int err = 0;

        err = clEnqueueWriteBuffer(m_opencl_mgr->m_command_queue, m_mem_rcounter, CL_TRUE, 0, m_list_size * sizeof(cl_double), m_rcounter, 0, NULL, NULL);

        if (err != CL_SUCCESS)
        {
            std::cout << "Error: Failed to write to source array!" << std::endl;
            return 0;
        }

        return 1;
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
        cl_int err = 0;
        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &m_mem_noise);
        err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &m_mem_rcounter);

        if (err != CL_SUCCESS)
        {
            std::cout << "Error: Failed to set kernel arguments! " << err << std::endl;
            return 0;
        }
        return 1;
    }

        /**
    * Sets OpenCL kernel arguments.
    *
    * @param	commands	OpenCL command queue
    * @param	list_size	Number of data points to be written in the OpenCL memory buffers
    *
    * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
    */
    int set_kernel_args(cl_kernel kernel)
    {
        // Set the arguments to the compute kernel
        cl_int err = 0;
        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &m_mem_t0);
        err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &m_mem_y0);
        err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &m_mem_params);
        err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &m_mem_rcounter);

        if (err != CL_SUCCESS)
        {
            std::cout << "Error: Failed to set kernel arguments! " << err << std::endl;
            return 0;
        }
        return 1;
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
        //m_opencl_mgr->build_program(m_programs.at(1));

        // Create the OpenCL buffers
        m_num_noi = 3;
        create_buffers(m_opencl_mgr->m_context, 10, 3, 3);

        // Create the OpenCL kernels
        // One for the noise numbers generation and one for the integration method

        cl_kernel temp_kernel = m_opencl_mgr->create_kernel("random_numbers", m_programs.at(0));
        if (temp_kernel==NULL)
        {
            std::cout << "FU" << std::endl;
        }
        m_kernels.push_back(temp_kernel);

        // temp_kernel = m_opencl_mgr->create_kernel("solver_caller", m_programs.at(1));
        // if (temp_kernel==NULL)
        // {
        //     std::cout << "FU" << std::endl;
        // }
        // m_kernels.push_back(temp_kernel);


        // Assign arguments to the kernels
        set_kernel_args_noise(m_kernels.at(0));

		return 0;
    }

    /**
     * @brief Executes the ODE solver on the selected OpenCL device.
     * 
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    int run_solver()
    {
        write_noise_buffer();

        std::cout << "Run 0" << std::endl;

        size_t global = size_t(20);
        size_t local;

        cl_int err;
        err = clEnqueueNDRangeKernel(m_opencl_mgr->m_command_queue, m_kernels.at(0), 1, NULL, &global, NULL, 0, NULL, NULL);

        //std::cout << err << std::endl;

        std::cout << "Run 1" << std::endl;

		clFinish(m_opencl_mgr->m_command_queue);
        std::cout << "Run 2" << std::endl;

        cl_double* orbits_out = new cl_double[m_num_noi * m_kernel_steps * m_list_size];

        err = clEnqueueReadBuffer(m_opencl_mgr->m_command_queue, m_mem_noise, CL_TRUE, 0, m_num_noi * m_kernel_steps * m_list_size * sizeof(cl_double), orbits_out, 0, NULL, NULL);
        clFinish(m_opencl_mgr->m_command_queue);
        
        std::cout << "Run 3" << std::endl;

        for (int i=0; i<10; i++)
        {
            std::cout << orbits_out[i] << std::endl;
        }

        std::cout << "Run 4" << std::endl;

        m_log->toFile();

        return 0;
    }
};
}

#endif // SODECL_EULER_HPP