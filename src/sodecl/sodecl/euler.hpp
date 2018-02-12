#ifndef SODECL_EULER_HPP
#define SODECL_EULER_HPP

#include "sodecl.hpp"

namespace sodecl
{

/**
 * @brief Abstract class that acts as an interface for the implementation of different ODE or SDE solvers.
 * 
 */
class euler : public solver_interface
{
    /********************************************************************************************
    ODE SOLVERS SECTION VARIABLES
    */

    char*			m_ode_system_string;			/**< ODE system OpenCL function */
    solver_Type		m_solver;						/**< ODE solver type */ 
    double			m_dt;							/**< Solver (initial) time step in seconds */ 
    double			m_int_time;						/**< Time units to integrate the ODE system */ 
    int				m_kernel_steps;					/**< Number of step the solver with perform in each kernel call */ 
    int				m_num_dt_steps;					/**< Number of dt steps for the state of the ODE system to be saved */
    int				m_list_size;					/**< Number of ODE system parameter combinations */ 
    int				m_num_equat;					/**< Number of ODE system equations */ 
    int				m_num_params;					/**< Number of ODE system parameters */ 
    output_Type		m_output_type;					/**< Type of output (binary file or array) */ 
    int*			m_outputPattern;				/**< Array which holds the output pattern. E.g. if the system has 3 equations the array {1,2,3}   
                                                        will write to the output file the results of the 1st then 2nd and then 3rd state variable */

    /**
     * @brief Default constructor.
     * 
     */
    euler() : solver_interface()
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