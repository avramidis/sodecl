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

std::vector<cl_double> sodeclcall(std::vector<double> &a_t0,
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
    sodecl::solver_type a_solver;
    switch (a_solver_type)
    {
    case 0:
        a_solver = sodecl::solver_type::STOCHASTICEULER;
        break;
    case 1:
        a_solver = sodecl::solver_type::EULER;
        break;
    case 2:
        a_solver = sodecl::solver_type::RUNGEKUTTA;
        break;
    case 3:
        a_solver = sodecl::solver_type::IMPLICITEULER;
        break;
    case 4:
        a_solver = sodecl::solver_type::IMPLICITMIDPOINT;
        break;
    default:
        std::cout << "Unknown SDE solver selection." << std::endl;
        std::vector<cl_double> myvector(0);
        return myvector;
    }

    sodecl::opencl_mgr* m_opencl_mgr = new sodecl::opencl_mgr;
    m_opencl_mgr->choose_opencl_device(a_platform, sodecl::device_Type::ALL, a_device);
    int err = m_opencl_mgr->create_opencl_context();    
    err = m_opencl_mgr->create_command_queue();
    if (err != 1)
    {
        std::cout << "Error: Failed to create command queue!" << std::endl;
    }

    sodecl::stochastic_euler* m_stochastic_euler = new sodecl::stochastic_euler(m_opencl_mgr,
                                                                                "kernels",
                                                                                &a_system[0],
                                                                                a_dt,
                                                                                a_tspan,
                                                                                a_ksteps,
                                                                                a_equats,
                                                                                a_nparams,
                                                                                a_nnoi,
                                                                                a_orbits,
                                                                                a_y0.data(),
                                                                                a_params.data());

    m_stochastic_euler->create_kernel_strings();

    m_stochastic_euler->setup_solver();

    m_stochastic_euler->run_solver();

    return m_stochastic_euler->m_output;
}

PYBIND11_MODULE(sodecl_interface, m)
{
    m.doc() = "sodecl plugin";

    m.def("sodeclcall", &sodeclcall, "A function that integrates a SDE/ODE system.");
}