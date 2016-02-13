# odecl

ODE OpenCL solvers.

Example:

```C++

// Set the options of the ODE Solver
int list_size = 256;		// Number of different model orbits to calculate
int equat_num = 6;		// ODE system number of equations
int param_num = 6;		// ODE system number of parameters

odecl::system *mysystem = new odecl::system("odecl\odecl\kernels", "broomhead.cl", odecl::solver_Type::RungeKutta, 1e-6, 6, 400, equat_num, param_num, list_size);

mysystem->choose_device(0, odecl::device_Type::CPU, 0);

cl_double *t0 = new cl_double[list_size];
mysystem->read_data_from_file("x_t0.txt", t0);
mysystem->set_t0(t0);

cl_double *y0 = new cl_double[list_size*equat_num];
mysystem->read_data_from_file("x_y0.txt", y0);
mysystem->set_y0(y0);

cl_double *params = new cl_double[list_size*param_num];
mysystem->read_data_from_file("x_params.txt", params);
mysystem->set_params(params);

// Setup and run the ODE solver
mysystem->setup_ode_solver();

mysystem->~system();

```
