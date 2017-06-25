//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include "iostream"
#include <sodecl.hpp>
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[])
{
	//sodecl::timer start_timer;

	int a_platform;											 /**< platform - OpenCL platform number */

	// device - OpenCL device number of selected platform
	int a_device;

	// Kernel filename with the ODE system definition
	string a_system;

	// initial - Initial conditions for each orbit for the ODE system
	string a_initial;

	// parameters - Parameters sets for the ODE system for all orbits
	string a_parameters;

	// odesolver - ODE solver
	sodecl::solver_Type a_solver;

	// orbits - Number of orbits to be integrated
	int a_orbits;

	// nequats - Number of equations of the ODE system
	int a_equats;
		
	// nparams - Number of parameters of the ODE system
	int a_nparams;

	// nnoi - Number of noise processes
	int a_nnoi;

	// dt - ODE solver time step size
	double a_dt;

	// tspan - Integration time span
	double a_tspan;

	// ksteps - Number of ODE solvers executed in each OpenCL kernel call
	int a_ksteps;

	// local_group_size - OpenCL local group size
	int a_local_group_size;

	if (argc > 1)
	{
		// platform   - OpenCL platform number
		a_platform = atoi(argv[1]);

		// device - OpenCL device number of selected platform
		a_device = atoi(argv[2]);

		// Kernel filename with the ODE system definition
		a_system = argv[3];

		// initial - Initial conditions for each orbit for the SDE system
		a_initial = argv[4];

		// parameters - Parameters sets for the SDE system for all orbits
		a_parameters = argv[5];

		// odesolver - ODE solver
		switch (atoi(argv[6]))
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
			return 0;
		}
		
		// orbits - Number of orbits to be integrated
		a_orbits = atoi(argv[7]);

		// nequats - Number of equations of the SDE system
		a_equats = atoi(argv[8]);

		// nparams - Number of parameters of the SDE system
		a_nparams = atoi(argv[9]);

		// nnoi - Number of noise processes
		a_nnoi = atoi(argv[10]);

		// dt - SDE solver time step size
		a_dt = atof(argv[11]);

		// tspan - Integration time span
		a_tspan = atof(argv[12]);

		// ksteps - Number of SDE solvers executed in each OpenCL kernel call
		a_ksteps = atoi(argv[13]);

		// local_group_size - OpenCL local group size
		a_local_group_size = atoi(argv[14]);
	}
	else
	{
		std::cout << "Number of argument is wrong." << std::endl;
		std::cout << "Argument list:" << std::endl;
		std::cout << "Argument 1  - OpenCL platform number" << std::endl;
		std::cout << "Argument 2  - OpenCL device number of selected platform" << std::endl;
		std::cout << "Argument 3  - Kernel filename with the SDE system definition" << std::endl;
		std::cout << "Argument 4  - Initial conditions for each orbit for the SDE system" << std::endl;
		std::cout << "Argument 5  - Parameters sets for the SDE system for all orbits" << std::endl;
		std::cout << "Argument 6  - SDE solver" << std::endl;
		std::cout << "Argument 7  - Number of orbits to be integrated" << std::endl;
		std::cout << "Argument 8  - Number of equations of the SDE system" << std::endl;
		std::cout << "Argument 9  - Number of parameters of the SDE system" << std::endl;
		std::cout << "Argument 10 - Number of noise processes" << std::endl;
		std::cout << "Argument 11 - SDE solver time step size" << std::endl;
		std::cout << "Argument 12 - Integration time span" << std::endl;
		std::cout << "Argument 13 - Number of SDE solvers executed in each OpenCL kernel call" << std::endl;
		std::cout << "Argument 14 - OpenCL local group size" << std::endl;

		return 0;
	}

	sodecl::sodeclmgr *mysodeclmgr = new sodecl::sodeclmgr("kernels", &a_system[0], a_solver, a_dt, a_tspan, a_ksteps, a_equats, a_nparams, a_nnoi, a_orbits, sodecl::output_Type::File);

	mysodeclmgr->set_outputfile("sodecloutput.bin");

	int success;

	// Choose device
	success = mysodeclmgr->choose_device(a_platform, sodecl::device_Type::ALL, a_device);
	if (success == 0)
	{
		cerr << "Error selecting OpenCL device!" << endl;
	}

	cl_double *t0 = new cl_double[a_orbits];
	success = mysodeclmgr->read_binary_data_from_file("x_t0.bin", t0);
	//success = mysystem->read_data_from_file("x_t0.txt", t0);
	if (success == 0)
	{
		cerr << "Error reading data from file: x_t0.txt !" << endl;
	}
	mysodeclmgr->set_t0(t0);

	cl_double *y0 = new cl_double[a_orbits*a_equats];
	success = mysodeclmgr->read_binary_data_from_file("x_y0.bin", y0);
	//success = mysystem->read_data_from_file("x_y0.txt", y0);
	if (success == 0)
	{
		cerr << "Error reading data from file: x_y0.txt !" << endl;
	}
	mysodeclmgr->set_y0(y0);

	cl_double *params = new cl_double[a_orbits*a_nparams];
	success = mysodeclmgr->read_binary_data_from_file("x_params.bin", params);
	//success = mysystem->read_data_from_file("x_params.txt", params);
	if (success == 0)
	{
		cerr << "Error reading data from file: x_params.txt !" << endl;
	}
	mysodeclmgr->set_params(params);

	// Set the local group size.
	mysodeclmgr->set_local_group_size(a_local_group_size);

	// Setup and run the ODE solver
	mysodeclmgr->setup_ode_solver();

	mysodeclmgr->run_ode_solver();

	delete mysodeclmgr;

	//delete t0;
	//delete y0;
	//delete params;

	_CrtDumpMemoryLeaks();

	return 0;
}