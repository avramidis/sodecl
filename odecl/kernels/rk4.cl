//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//




//static void first_step(double dt, double t, double y[_numeq_], double yout[_numeq_], double k1[_numeq_], double p[_numpar_], double x2[_numeq_])
//{
//	// k2
//	//double x2[_numeq_];
//
//	for (int i = 0; i < _numeq_; i++)
//	{
//		x2[i] = y[i] + (dt * 0.50)*k1[i];
//	}
//
//	ode_system(t + 0.50*dt, x2, k1, p);
//
//	for (int i = 0; i < _numeq_; i++)
//	{
//		yout[i] = yout[i] + 2.00 * k1[i];
//	}
//}




/// <summary>
/// Performs a single step for the fourth-order Runge-kutta method.
/// </summary>
/// <param name="dt">The dt is the time step.</param>
/// <param name="t">The t is the current time value.</param>
/// <param name="y">The y is the current phase values.</param>
/// <param name="yout">The yout is the next phase values.</param>
/// <param name="p">The p is the parameter values.</param>
inline static void ode_solver(double dt, double t, double y[_numeq_], double yout[_numeq_], double p[_numpar_])
{
	ode_system(t, y, yout, p);

	// k1
	double k1[_numeq_];
	int i;

	for (i = 0; i < _numeq_; i++)
	{
		k1[i] = yout[i];
	}
	//barrier(CLK_LOCAL_MEM_FENCE);

	//double x2[_numeq_];
	//first_step(dt, t, y, yout, k1, p, x2);

	// k2
	double x2[_numeq_];

	for (i = 0; i < _numeq_; i++)
	{
		x2[i] = y[i] + (dt * 0.50)*k1[i];
	}
	
	ode_system(t + 0.50*dt, x2, k1, p);

	for (i = 0; i < _numeq_; i++)
	{
		yout[i] = yout[i] + 2.00 * k1[i];
	}

	// k3

	for (i = 0; i < _numeq_; i++)
	{
		x2[i] = y[i] + (dt * 0.50)*k1[i];
	}
	//double k3[_numeq_];
	ode_system(t + 0.50*dt, x2, k1, p);

	for (i = 0; i < _numeq_; i++)
	{
		yout[i] = yout[i] + 2.00 * k1[i];
	}

	// k4
	for (i = 0; i < _numeq_; i++)
	{
		x2[i] = y[i] + dt*k1[i];
	}
	ode_system(t+dt, x2, k1, p);

	for (i = 0; i < _numeq_; i++)
	{
		yout[i] = y[i] + (yout[i] + k1[i]) * dt / 6.00;
	}

	////detterm[0] = y[0]; // it's the time variable
	//for (int i = 0; i < _numeq_; i++)
	//{
	//	yout[i] = y[i] + (k1[i] + 2.00 * k2[i] + 2.00 * k3[i] + k4[i]) * dt / 6.00;
	//}
}