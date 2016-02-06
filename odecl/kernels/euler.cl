//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//


// Euler solver
inline static void ode_solver(double dt, double t, double y[_numeq_], double yout[_numeq_], double p[_numpar_])
{
	ode_system(t, y, yout, p);

	for (int i = 0; i < _numeq_; i++)
	{
		yout[i] = y[i] + yout[i] * dt;
	}
}