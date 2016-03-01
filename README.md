# odecl

ODE OpenCL solvers.

Example:

```C++

// Set the options of the ODE Solver
int list_size = 256;		// Number of different model orbits to calculate
int equat_num = 6;		// ODE system number of equations
int param_num = 6;		// ODE system number of parameters

odecl::system *mysystem = new odecl::system("odecl/odecl/kernels", "broomhead.cl", 
                    odecl::solver_Type::RungeKutta, 1e-6, 6, 400, equat_num, param_num, list_size);

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

Code for the Broomhead et al. (2000) model. This is the broomhead.cl file.

```C++
#define T1 0.15
#define T2 0.012
#define Tn 25.00

inline static double sburstf(double x, double aval, double bval, double atval, double btval)
{
	double y;
	y = 0;

	if (x > 0)
	{
		y = (atval * (1.00 - exp(-x / btval)));
    }
	else
	{
		y = (-(aval / bval) * x * exp(x / bval));
	}

	return y;
}

inline static double dsburstf(double x, double aval, double bval, double atval, double btval)
{
	double y;
	y = 0;

	if (x > 0)
	{
		y = (atval*exp(-x / btval)) / btval;
	}
	else
	{
		y = -((aval*exp(x / bval)) / bval + (aval*x*exp(x / bval)) / (bval*bval));
	}
	return y;
}

inline static void calc_jacobian(double y[_numeq_], double jac[_numeq_][_numeq_], double p[_numpar_])
{
	jac[0][0] = 0;
	jac[0][1] = 1;
	jac[0][2] = 0;
	jac[0][3] = 0;
	jac[0][4] = 0;
	jac[0][5] = 0;

	jac[1][0] = -1 / (T1*T2);
	jac[1][1] = -(1 / T1) - (1 / T2);
	jac[1][2] = 1 / (T1*T2);
	jac[1][3] = (1 / T1) + (1 / T2);
	jac[1][4] = -(1 / T1) - (1 / T2);
	jac[1][5] = 0;

	jac[2][0] = 0;
	jac[2][1] = 0;
	jac[2][2] = -1 / Tn;
	jac[2][3] = 1;
	jac[2][4] = -1;
	jac[2][5] = 0;

	jac[3][0] = 0;
	jac[3][1] = 0;
	jac[3][2] = 0;
	jac[3][3] = -(p[3] * (y[4] * y[4]) + 1) / p[2];
	jac[3][4] = -(2 * p[3] * y[4] * y[3]) / p[2];
	jac[3][5] = dsburstf(y[5], p[0], p[1], p[4], p[5]) / p[2];

	jac[4][0] = 0;
	jac[4][1] = 0;
	jac[4][2] = 0;
	jac[4][3] = -(2 * p[3] * y[4] * y[3]) / p[2];
	jac[4][4] = -(p[3] * (y[3] * y[3]) + 1) / p[2];
	jac[4][5] = dsburstf(-y[5], p[0], p[1], p[4], p[5]) / p[2];

	jac[5][0] = 0;
	jac[5][1] = 0;
	jac[5][2] = 0;
	jac[5][3] = -1;
	jac[5][4] = 1;
	jac[5][5] = 0;
}

inline static void ode_system(double t, double y[_numeq_], double yout[_numeq_], double p[_numpar_])
{
	// g
	yout[0] = y[1];
	// u
	yout[1] = -(((1.00 / T1) + (1.00 / T2)) * y[1]) - (1.00 / (T1 * T2) * y[0]) + (1.00 / (T1 * T2) * y[2]) + ((1.00 / T1) + (1.00 / T2)) * (y[3] - y[4]);
	// n
	yout[2] = -(y[2] / Tn) + (y[3] - y[4]);
	// r
	yout[3] = (1.00 / p[2])*(-y[3] - (p[3] * y[3] * y[4] * y[4]) + sburstf(y[5], p[0], p[1], p[4], p[5]));
	// l
	yout[4] = (1.00 / p[2])*(-y[4] - (p[3] * y[4] * y[3] * y[3]) + sburstf(-y[5], p[0], p[1], p[4], p[5]));
	// m
	yout[5] = -(y[3] - y[4]);
}
```
