//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//


// StochasticEuler solver
inline static void ode_solver(double dt, double t, double y[_numeq_], double yout[_numeq_], double p[_numpar_], double noise[_numnoi_])
{
	ode_system(t, y, yout, p);

	double stoch[_numeq_];
	ode_system_stoch(t, y, stoch, p, noise);

	for (int i = 0; i < _numeq_; i++)
	{
		yout[i] = y[i] + yout[i] * dt + stoch[i];
	}
}