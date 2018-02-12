#ifndef SODECL_EULER_HPP
#define SODECL_EULER_HPP

#include "sodecl.hpp"

namespace sodecl
{

/**
 * @brief Class that implements the functions needed for the Euler solver.
 * 
 */
class euler : public solver_interface
{
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
    euler(string        kernel_path_str,
          char*         ode_system_str,
          double        dt,
          double        int_time,
          int           kernel_steps,
          int           num_equat,
          int           num_params,
          int           list_size,
          output_Type   output_type) : solver_interface(kernel_path_str,
                                                      ode_system_str,
                                                      dt,
                                                      int_time,
                                                      kernel_steps,
                                                      num_equat,
                                                      num_params,
                                                      list_size,
                                                      output_type)
    {
    }

    /**
     * @brief Virtual destructor.
     * 
     */
    ~euler()
    {
    }

    /**
     * @brief Forms the OpenCL kernel string(s).
     * 
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    int create_kernel_string()
    {
        // Create the parameters section of the kernel string. These parameter values are defines

        // List size
        add_string_to_kernel_sources("#define _listsize_ ");
        add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_list_size)));

        add_string_to_kernel_sources("\n");
        // Kernel steps
        add_string_to_kernel_sources("#define _numsteps_ ");
        add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_kernel_steps)));
        add_string_to_kernel_sources("\n");

        //cout << "Kernel steps done" << endl;

        // ODE solver time size
        add_string_to_kernel_sources("#define _m_dt_ ");
        //add_string_to_kernel_sources(std::to_string(static_cast<long double>(m_dt)));
        add_string_to_kernel_sources(double2string(m_dt));
        add_string_to_kernel_sources("\n");

        //cout << "ODE solver time size" << endl;

        // ODE system number of equations
        add_string_to_kernel_sources("#define _numeq_ ");
        add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_num_equat)));
        add_string_to_kernel_sources("\n");

        // ODE system number of parameters
        add_string_to_kernel_sources("#define _numpar_ ");
        add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_num_params)));
        add_string_to_kernel_sources("\n");

        //cout << "ODE system number of parameters" << endl;

        // Read the ODE system functions
        read_kernel_file(m_ode_system_string);
        add_string_to_kernel_sources("\n");

        std::vector<char> kernelpath_char;
        string kernelpath = m_kernel_path_str;

        //std::cout << "The kernal path is: " << kernelpath << std::endl;

        //cout << "Read the Euler solver" << endl;
        kernelpath.append("/euler.cl");

        for (int i = 0; i < kernelpath.size(); i++)
        {
            kernelpath_char.push_back(kernelpath[i]);
        }
        kernelpath_char.push_back('\0');

        // m_log->write(&*kernelpath_char.begin());
        // m_log->write("\n");

        kernelpath_char.shrink_to_fit();

        read_kernel_file(&*kernelpath_char.begin());

        add_string_to_kernel_sources("\n");

        // Read the solver
        string kernelsolverpath_char = m_kernel_path_str;

        //cout << "Read the Runge-Kutta solver" << endl;
        kernelsolverpath_char.append("/solver_caller.cl");

        //std::cout << kernelsolverpath_char << std::endl;
        read_kernel_file(&kernelsolverpath_char[0]);
        add_string_to_kernel_sources("\n");

        // Print the string
        //cout << m_kernel_sources.data() << endl;

        //m_kernel_sources.shrink_to_fit();
        m_source_size = m_kernel_sources.size();

        //cout << m_kernel_sources.data() << endl;

        // This is for debug purpose
        std::ofstream out("Generated_kernel.cl");
        out << m_kernel_sources.data();
        out.close();

        return 1;
    }

    /**
     * @brief Creates the OpenCL buffers needed for the solver.
     * 
     * @param	context		OpenCL cl_context.
     * @param	list_size	Size of the buffers in number of data points.
     * @param	equat_num	Number of equations.
     * @param   param_num	Number of parameters.
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    int create_buffers(cl_context context, int list_size, int equat_num, int param_num)
    {
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
    }

    /**
     * @brief Setups the selected ODE solver OpenCL kernel source.
     * 
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    int setup_solver()
    {
    }

    /**
     * @brief Executes the ODE solver on the selected OpenCL device.
     * 
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    int run_solver()
    {
    }
};
}

#endif // SODECL_EULER_HPP