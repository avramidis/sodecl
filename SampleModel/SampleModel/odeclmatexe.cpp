//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

//#define _SCL_SECURE_NO_WARNINGS

#include "iostream"
#include <odecl.hpp>

#include <stdio.h>

using namespace std;


int main(int argc, char* argv[])
{
	// platform   - OpenCL platform number
	int a_platform;

	// device - OpenCL device number of selected platform
	int a_device;

	// Kernel filename with the ODE system definition
	string a_system;

	// initial - Initial conditions for each orbit for the ODE system
	string a_initial;

	// parameters - Parameters sets for the ODE system for all orbits
	string a_parameters;

	// odesolver - ODE solver
	odecl::solver_Type a_solver;

	// orbits - Number of orbits to be integrated
	int a_orbits;

	// nequats - Number of equations of the ODE system
	int a_equats;
		
	// nparams - Number of parameters of the ODE system
	int a_nparams;

	// dt - ODE solver time step size
	double a_dt;

	// tspan - Integration time span
	double a_tspan;

	// ksteps - Number of ODE solvers executed in each OpenCL kernel call
	int a_ksteps;

	if (argc > 1)
	{
		// platform   - OpenCL platform number
		a_platform = atoi(argv[1]);

		// device - OpenCL device number of selected platform
		a_device = atoi(argv[2]);

		// Kernel filename with the ODE system definition
		a_system = argv[3];

		// initial - Initial conditions for each orbit for the ODE system
		a_initial = argv[4];

		// parameters - Parameters sets for the ODE system for all orbits
		a_parameters = argv[5];

		// odesolver - ODE solver
		switch (atoi(argv[6]))
		{
		case 0:
			a_solver = odecl::solver_Type::Euler;
			break;
		case 1:
			a_solver = odecl::solver_Type::RungeKutta;
			break;
		case 2:
			a_solver = odecl::solver_Type::ImplicitEuler;
			break;
		case 3:
			a_solver = odecl::solver_Type::ImplicitMidpoint;
			break;
		default:
			std::cout << "Unknown ODE solver selection." << std::endl;
			return 0;
		}
		

		// orbits - Number of orbits to be integrated
		a_orbits = atoi(argv[7]);

		// nequats - Number of equations of the ODE system
		a_equats = atoi(argv[8]);

		// nparams - Number of parameters of the ODE system
		a_nparams = atoi(argv[9]);

		// dt - ODE solver time step size
		a_dt = atof(argv[10]);

		// tspan - Integration time span
		a_tspan = atof(argv[11]);

		// ksteps - Number of ODE solvers executed in each OpenCL kernel call
		a_ksteps = atoi(argv[12]);
	}
	else
	{
		std::cout << "Number of argument is wrong." << std::endl;
		std::cout << "Argument list:" << std::endl;
		std::cout << "Argument 1  - OpenCL platform number" << std::endl;
		std::cout << "Argument 2  - OpenCL device number of selected platform" << std::endl;
		std::cout << "Argument 3  - Kernel filename with the ODE system definition" << std::endl;
		std::cout << "Argument 4  - Initial conditions for each orbit for the ODE system" << std::endl;
		std::cout << "Argument 5  - Parameters sets for the ODE system for all orbits" << std::endl;
		std::cout << "Argument 6  - Number of orbits to be integrated" << std::endl;
		std::cout << "Argument 7  - Number of equations of the ODE system" << std::endl;
		std::cout << "Argument 8  - Number of parameters of the ODE system" << std::endl;
		std::cout << "Argument 9  - ODE solver time step size" << std::endl;
		std::cout << "Argument 10 - Integration time span" << std::endl;
		std::cout << "Argument 11 - Number of ODE solvers executed in each OpenCL kernel call" << std::endl;
	}

	odecl::system *mysystem = new odecl::system("C:/Workfolder/ODECL_Repositories/odecl/odecl/kernels", &a_system[0], a_solver, a_dt, a_tspan, a_ksteps, a_equats, a_nparams, a_orbits, odecl::output_Type::File);

	mysystem->set_outputfile("odecloutput.bin");

	//odecl::system *mysystem = new odecl::system("C:/Workfolder/ODECL_Repositories/odecl/odecl/kernels", "broomhead.cl", odecl::solver_Type::RungeKutta, 1e-6, 6, 400, equat_num, param_num, list_size, odecl::output_Type::File);


	//odecl::system *mysystem = new odecl::system("broomhead.cl", odecl::solver_Type::Euler, 1e-7, 6, 4000, equat_num, param_num, list_size);
	//odecl::system *mysystem = new odecl::system("broomhead.cl", odecl::solver_Type::RungeKutta, 1e-6, 6, 400, equat_num, param_num, list_size);

	//odecl::system *mysystem = new odecl::system(solver, dt, int_time, kernel_steps, num_equat, num_params, list_size);

	int success;

	// Choose device
	success = mysystem->choose_device(a_platform, odecl::device_Type::CPU, a_device);
	if (success == 0)
	{
		cerr << "Error selecting OpenCL device!" << endl;
	}

	cl_double *t0 = new cl_double[a_orbits];
	success = mysystem->read_data_from_file("x_t0.txt", t0);
	if (success == 0)
	{
		cerr << "Error reading data from file: x_t0.txt !" << endl;
	}
	mysystem->set_t0(t0);

	cl_double *y0 = new cl_double[a_orbits*a_equats];
	success = mysystem->read_data_from_file("x_y0.txt", y0);
	if (success == 0)
	{
		cerr << "Error reading data from file: x_y0.txt !" << endl;
	}
	mysystem->set_y0(y0);

	cl_double *params = new cl_double[a_orbits*a_nparams];
	success = mysystem->read_data_from_file("x_params.txt", params);
	if (success == 0)
	{
		cerr << "Error reading data from file: x_params.txt !" << endl;
	}
	mysystem->set_params(params);

	// Setup and run the ODE solver
	mysystem->setup_ode_solver();

	mysystem->run_ode_solver();

	mysystem->~system();

	//system("pause");

	return 0;
}