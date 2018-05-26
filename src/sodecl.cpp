//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#ifdef _DEBUG
//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
//#endif

#include "iostream"
#include <sodecl.hpp>
#include <stdio.h>
#include <math.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>

using namespace std;

std::vector<cl_double> sodeclcall( std::vector<double> &a_t0,
                std::vector<double> &a_y0,
                std::vector<double> &a_params,
                int a_platform, 
                int a_device, 
                string a_system, 
                int a_solver_type, 
                int a_orbits, 
                int a_equats, 
                int a_nparams, 
                int a_nnoi, 
                double a_dt,
                double a_tspan, 
                int a_ksteps, 
                int a_local_group_size)
{
    // odesolver - ODE solver
    sodecl::solver_Type a_solver;
    switch (a_solver_type)
    {
    case 0:
        a_solver = sodecl::solver_Type::StochasticEuler;
        break;
    case 1:
        a_solver = sodecl::solver_Type::Euler;
        break;
    case 2:
        a_solver = sodecl::solver_Type::RungeKutta;
        break;
    case 3:
        a_solver = sodecl::solver_Type::ImplicitEuler;
        break;
    case 4:
        a_solver = sodecl::solver_Type::ImplicitMidpoint;
        break;
    default:
        std::cout << "Unknown SDE solver selection." << std::endl;
        std::vector<cl_double> myvector(0);
        return myvector;
    }

	sodecl::sodeclmgr *mysodeclmgr = new sodecl::sodeclmgr("kernels", 
															&a_system[0], 
															a_solver, 
															a_dt, 
															a_tspan, 
															a_ksteps, 
															a_equats, 
															a_nparams, 
															a_nnoi, 
															a_orbits, 
															sodecl::output_Type::File);

	mysodeclmgr->set_outputfile("sodecloutput.bin");

	int success;

	// Choose device
	success = mysodeclmgr->choose_device(a_platform, sodecl::device_Type::ALL, a_device);
	if (success == 0)
	{
		cerr << "Error selecting OpenCL device!" << endl;
	}

	mysodeclmgr->set_t0(a_t0.data());
	mysodeclmgr->set_y0(a_y0.data());
	mysodeclmgr->set_params(a_params.data());

	// Set the local group size.
	mysodeclmgr->set_local_group_size(a_local_group_size);

	// Setup and run the ODE solver
	int ret = mysodeclmgr->setup_ode_solver();
    if (ret == 0)
    {
        std::vector<cl_double> myvector(0);
        return myvector;
    }
    
	mysodeclmgr->run_ode_solver();


    return mysodeclmgr->m_output;

    // The delete mysodeclmgr call breaks pybind11 for some reason
	//delete mysodeclmgr;

	//return 1;
}

PYBIND11_MODULE(sodecl, m) {
    m.doc() = "sodecl plugin"; // optional module docstring

    m.def("sodeclcall", &sodeclcall, "A function that integrates a SDE/ODE system.");
}