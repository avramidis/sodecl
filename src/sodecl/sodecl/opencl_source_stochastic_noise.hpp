//---------------------------------------------------------------------------//
// Copyright (c) 2015,2018 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef SODECL_OPENLC_SOURCE_STOCHASTIC_NOISE_HPP
#define SODECL_OPENLC_SOURCE_STOCHASTIC_NOISE_HPP

#include "sodecl.hpp"
#include "sodecl/opencl_source_mgr.hpp"

namespace sodecl
{

/**
 * @brief Abstract class that implements the functions needed for the creation of OpenCL source files.
 * 
 */
class opencl_source_stochastic_noise : protected opencl_source_mgr
{
  private:
    /********************************************************************************************
	* GENERAL VARIABLES
	*/
    string			m_kernel_path_str;		        /**< OpenCL kernels solvers path */
    double			m_dt;							/**< Solver (initial) time step in seconds */ 

  public:
    /**
     * @brief Default constructor.
     * 
     */
    opencl_source_stochastic_noise(string kernel_path_str, double dt)
    { 
        m_kernel_path_str = kernel_path_str;
        m_dt = dt;
    };

    /**
     * @brief Destructor.
     * 
     */
    ~opencl_source_stochastic_noise(){ };

    /**
    * Form the OpenCL kernel string.
    *
    * @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
    */
    int create_kernel_string()
    {
        // Create the parameters section of the kernel string. These parameter values are defines
        // SODE solver time size
        add_string_to_kernel_sources("#define _m_dt_ ");
        add_string_to_kernel_sources(double_to_string(m_dt));
        add_string_to_kernel_sources("\n");

        std::vector<char> kernelpath_char;
        string kernelpath = m_kernel_path_str;
        kernelpath.append("/random_numbers.cl");

        for (int i = 0; i < kernelpath.size(); i++)
        {
            kernelpath_char.push_back(kernelpath[i]);
        }
        kernelpath_char.push_back('\0');
        
        kernelpath_char.shrink_to_fit();

        read_kernel_file(&*kernelpath_char.begin());
        add_string_to_kernel_sources("\n");
        m_source_size = m_kernel_sources.size();
        
        // This is for debug purpose
        std::ofstream out("Generated_kernel_noise.cl");
        out << m_kernel_sources.data();
        out.close();

        return 1;
    }
};
}

#endif // SODECL_OPENLC_SOURCE_STOCHASTIC_NOISE_HPP