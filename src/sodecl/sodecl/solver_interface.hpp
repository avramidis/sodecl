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
  public:

    /********************************************************************************************
    ODE SOLVER SECTION VARIABLES
    */
    
    char*			m_ode_system_string;			/**< ODE system OpenCL function */ 
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
    
    /********************************************************************************************
    OPENCL SOFTWARE SECTION VARIABLES
    */

    std::vector<cl_context>			m_contexts;				/**< OpenCL command contexts vector */
    std::vector<cl_command_queue>	m_command_queues;		/**< OpenCL command queues vector */
    std::vector<char>				m_kernel_sources;		/**< Char vector which stores the OpenCL kernel source string. @todo store multiple kernel source strings */
    std::string						m_build_options_str;	/**< Char vector which stores the OpenCL build options string */
    std::vector<build_Option>		m_build_options;		/**< build_Option vector which stores the OpenCL build options selection */
    char*							m_source_str;			/**< OpenCL kernel string */
    string							m_kernel_path_str;		/**< OpenCL kernels solvers path */
    size_t							m_source_size;			/**< OpenCL kernel string size */
    std::vector<cl_program>			m_programs;				/**< OpenCL programs vector */
    std::vector<cl_kernel>			m_kernels;				/**< OpenCL kernels vector */
    int								m_local_group_size;		/**< OpenCL device local group size */

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
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    virtual int create_kernel_string() = 0;

    /**
     * @brief Creates the OpenCL buffers needed for the solver.
     * 
     * @param	context		OpenCL cl_context.
     * @param	list_size	Size of the buffers in number of data points.
     * @param	equat_num	Number of equations.
     * @param   param_num	Number of parameters.
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    virtual int create_buffers(cl_context context, int list_size, int equat_num, int param_num) = 0;

    /**
     * @brief Writes data in the OpenCL memory buffers.
     * 
     * @param	commands	OpenCL command queue.
     * @param	list_size	Number of data points to be written in the OpenCL memory buffers.
     * @param	equat_num	Number of equations of the ODE or SDE system.
     * @param	param_num	Number of parameters of the ODE or SDE system.
     * @return  int         Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    virtual int write_buffers(cl_command_queue commands, int list_size, int equat_num, int param_num) = 0;

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
    
    /**
     * @brief Reads an OpenCL function or kernel file.
     * 
     * @param   filename    OpenCL file to read.
     */
    void read_kernel_file(char *filename)
    {
        std::ifstream t(filename);

        if (t.is_open())
        {
            t.seekg(0, std::ios::end);
            size_t size = t.tellg();
            std::string str(size, ' ');
            t.seekg(0);
            t.read(&str[0], size);
            t.close();

            m_source_size = str.length();

            for (int i = 0; i < m_source_size; i++)
            {
                m_kernel_sources.push_back(str[i]);
            }
        }
        else
        {
            std::cerr << "Error opening file";
        }
    }

    /**
     * @brief Adds the string str to the vector which stores the kernel string (i.e. OpenCL kernel code).
     * 
     * @param   str     The sting to be pushed in the OpenCL kernel code vector.
     * @return  int     Returns 1 if the operations were succcessful or 0 if they were unsuccessful.
     */
    int add_string_to_kernel_sources(string str)
    {
        // TODO: Add code to catch an exception in case the operation fails
        for (size_t i = 0; i < str.length(); i++)
        {
            m_kernel_sources.push_back(str[i]);
        }

        return 1;
    }

    /**
     * @brief Converts a double to string.
     * 
     * @param   x           Double number which will be coverted to string.
     * @return  string      A string of the converted double number.
     */
    string double2string(double x)
    {
        // @todo Add code to catch an exception in the unlikely case of the conversion fails
        std::string s;
        {
            std::ostringstream ss;
            ss << x;
            s = ss.str();
        }
        return s;
    }
};
}

#endif // SODECL_SOLVERINTERFACE_HPP