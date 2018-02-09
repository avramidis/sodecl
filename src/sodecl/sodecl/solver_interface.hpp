#ifndef SODECL_SOLVERINTERFACE_HPP
#define SODECL_SOLVERINTERFACE_HPP

#include "sodecl.hpp"

namespace sodecl
{

/**
 * @brief Abstract class that acts as an interface for the implementation of different ODE or SDE solvers.
 * 
 */
class solver_interface
{
        /**
         * @brief Default constructor.
         * 
         */
        solver_interface(){};

        /**
         * @brief Virtual destructor.
         * 
         */
        virtual ~solver_interface(){};

        /**
         * @brief Forms the OpenCL kernel string(s).
         * 
         * @return int Returns 1 if the operations were succcessfull or 0 if they were unsuccessful.
         */
        virtual int create_kernel_string() = 0;

        /**
         * @brief Creates the OpenCL buffers needed for the solver.
         * 
         * @param	context		OpenCL cl_context.
         * @param	list_size	Size of the buffers in number of data points.
         * @param	equat_num	Number of equations.
         * @param       param_num	Number of parameters.
         * @return      int             Returns 1 if the operations were succcessfull or 0 if they were unsuccessful.
         */
        virtual int create_buffers(cl_context context, int list_size, int equat_num, int param_num) = 0;

        /**
         * @brief Writes data in the OpenCL memory buffers.
         * 
         * @param	commands	OpenCL command queue.
	 * @param	list_size	Number of data points to be written in the OpenCL memory buffers.
	 * @param	equat_num	Number of equations of the ODE or SDE system.
	 * @param	param_num	Number of parameters of the ODE or SDE system.
         * @return      int             Returns 1 if the operations were succcessfull or 0 if they were unsuccessful.
         */
        virtual int write_buffers(cl_command_queue commands, int list_size, int equat_num, int param_num) = 0;

        /**
         * @brief Executes the ODE or SDE solver on the selected OpenCL device.
         * 
         * @return      int             Returns 1 if the operations were succcessfull or 0 if they were unsuccessful.
         */
        virtual int run_ode_solver() = 0;
};
}

#endif // SODECL_SOLVERINTERFACE_HPP