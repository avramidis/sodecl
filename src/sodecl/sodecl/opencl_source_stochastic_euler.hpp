//---------------------------------------------------------------------------//
// Copyright (c) 2015,2018 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef SODECL_OPENLC_SOURCE_STOCHASTIC_EULER_HPP
#define SODECL_OPENLC_SOURCE_STOCHASTIC_EULER_HPP

#include "sodecl.hpp"
#include "sodecl/opencl_source_interface.hpp"

namespace sodecl
{

/**
 * @brief Abstract class that implements the functions needed for the creation of OpenCL source files.
 * 
 */
class opencl_source_stochastic_euler : protected opencl_source_interface
{
  private:
    /********************************************************************************************
	* GENERAL VARIABLES
	*/
    string			m_kernel_path_str;		        /**< OpenCL kernels solvers path */
    double			m_dt;							/**< Solver (initial) time step in seconds */ 
    char*			m_ode_system_string;			/**< ODE or SDE system OpenCL function */
    int				m_kernel_steps;					/**< Number of step the solver with perform in each kernel call */ 
    int				m_num_equat;					/**< Number of ODE system equations */ 
    int				m_num_params;					/**< Number of ODE system parameters */ 
    int				m_num_noi;						/**< Number of noise processes */ 

  public:
    /**
     * @brief Default constructor.
     * 
     */
    opencl_source_stochastic_euler( string kernel_path_str, 
                                    double dt, 
                                    char* ode_system_string, 
                                    int kernel_steps, 
                                    int num_equat, 
                                    int num_params, 
                                    int num_noi)
    { 
        m_kernel_path_str = kernel_path_str;
        m_dt = dt;
        m_ode_system_string = ode_system_string;
        m_kernel_steps = kernel_steps;
        m_num_equat = num_equat;
        m_num_params = num_params; 
        m_num_noi = num_noi;
    };

    /**
     * @brief Destructor.
     * 
     */
    ~opencl_source_stochastic_euler(){ };

    /**
    * Form the OpenCL kernel string.
    *
    * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
    */
    std::string create_kernel_string()
    {
        // Create the parameters section of the kernel string. These parameter values are defines
        // Number of noise parameters
        add_string_to_kernel_sources("#define _numnoi_ ");
        add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_num_noi)));
        add_string_to_kernel_sources("\n");

        // Kernel steps
        add_string_to_kernel_sources("#define _numsteps_ ");
        add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_kernel_steps)));
        add_string_to_kernel_sources("\n");

        // ODE solver time size
        add_string_to_kernel_sources("#define _m_dt_ ");
        add_string_to_kernel_sources(double_to_string(m_dt));
        add_string_to_kernel_sources("\n");

        // ODE system number of equations
        add_string_to_kernel_sources("#define _numeq_ ");
        add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_num_equat)));
        add_string_to_kernel_sources("\n");

        // ODE system number of parameters
        add_string_to_kernel_sources("#define _numpar_ ");
        add_string_to_kernel_sources(std::to_string(static_cast<long long>(m_num_params)));
        add_string_to_kernel_sources("\n");

        // Read the SDE system functions
        read_kernel_file(m_ode_system_string);
        add_string_to_kernel_sources("\n");

        std::vector<char> kernelpath_char;
        string kernelpath = m_kernel_path_str;

        kernelpath.append("/stochasticeuler.cl");	// StochasticEuler

        for (size_t i = 0; i < kernelpath.size(); i++)
        {
            kernelpath_char.push_back(kernelpath[i]);
        }
        kernelpath_char.push_back('\0');
        
        kernelpath_char.shrink_to_fit();

        read_kernel_file(&*kernelpath_char.begin());

        add_string_to_kernel_sources("\n");

        // Read the solver 
        string kernelsolverpath_char = m_kernel_path_str;

        kernelsolverpath_char.append("/stochastic_solver_caller.cl");
        
        read_kernel_file(&kernelsolverpath_char[0]);
        add_string_to_kernel_sources("\n");

        //m_kernel_sources.shrink_to_fit();
        m_source_size = m_kernel_sources.size();
        
        // This is for debug purpose
        std::ofstream out("Generated_kernel_stochastic_euler.cl");
        out << m_kernel_sources.data();
        out.close();

        return std::string(m_kernel_sources.data());
    }
};
}

#endif // SODECL_OPENLC_SOURCE_STOCHASTIC_EULER_HPP