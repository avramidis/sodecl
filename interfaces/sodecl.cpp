#include "iostream"
#include <sodecl.hpp>
#include <stdio.h>
#include <math.h>
#include <vector>

using namespace std;

std::vector<double> sodeclcall( std::vector<double> &a_t0,
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
        std::vector<double> myvector(0);
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
															sodecl::output_Type::Array);

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

	// Setup and run the SODE solver
	int ret = mysodeclmgr->setup_sode_solver();
    if (ret == 0)
    {
        std::vector<double> myvector(0);
        return myvector;
    }
    
	mysodeclmgr->run_sode_solver();

    return mysodeclmgr->m_output;
}

//PYBIND11_MODULE(sodecl_interface, m) {
//    m.doc() = "sodecl plugin";
//
//    m.def("sodeclcall", &sodeclcall, "A function that integrates a SDE/ODE system.");
//}