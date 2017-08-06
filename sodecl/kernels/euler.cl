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

	for (int ieq = 0; ieq < _numeq_; ieq++)
	{
		yout[ieq] = y[ieq] + yout[ieq] * dt;
	}
}