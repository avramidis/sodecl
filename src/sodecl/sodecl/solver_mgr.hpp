//---------------------------------------------------------------------------//
// Copyright (c) 2015, 2018 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef SODECL_SOLVER_MGR_HPP
#define SODECL_SOLVER_MGR_HPP

#include "sodecl.hpp"

namespace sodecl
{

/**
 * @brief  Class that manages the SODECL integration methods needed by the ODE and SDE solvers.
 * 
 */
class solver_mgr
{
private:
  /********************************************************************************************
	* GENERAL VARIABLES
	*/

  opencl_mgr*       m_opencl_mgr;         /**< Pointer to the opencl_mgr object that implements the OpenCL functions needed by the ODE and SDE solvers. */

public:
  /**
     * @brief Default constructor.
     * 
     */
  solver_mgr(opencl_mgr* ocl_mgr)
  {
    m_opencl_mgr = ocl_mgr;
  };

  /**
     * @brief Destructor.
     * 
     */
  ~solver_mgr(){};

  /**
		* Create the object for the choosen the SDE or ODE integration method.
		*
		* @return	Returns 1 if the operations were succcessfull or 0 if they were unsuccessful
		*/
  int choose_solver(sodecl::solver_type solver, 
                    string kernel_path_str, 
                    double dt, 
                    char* ode_system_string, 
                    int kernel_steps, 
                    int num_equat, 
                    int num_params, 
                    int num_noi=0)
  {
    switch (solver)
    {
      case solver_type::STOCHASTICEULER:

        //opencl_source_stochastic_euler(kernel_path_str, dt, ode_system_string, kernel_steps, num_equat, num_params, num_noi);

        //stochastic_euler(kernel_path_str, ode_system_str, dt, int_time, kernel_steps, num_equat, num_params, num_noi, list_size,y0, params)
          
        break;
      
      case solver_type::EULER:
        break;

      case solver_type::RUNGEKUTTA:
        break;
      
      case solver_type::IMPLICITEULER:
        break;
      
      case solver_type::IMPLICITMIDPOINT:
        break;

      default:
        std::cout << "No valid solver chosen!" << std::endl;
    }
    return 1;
  }
};
} // namespace sodecl

#endif // SODECL_SOLVER_MGR_HPP