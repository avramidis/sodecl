//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//


// StochasticEuler solver
void sode_solver(double t, double y[_numeq_], double yout[_numeq_], double p[_numpar_], double noise[_numnoi_])
{
	sode_system(t, y, yout, p);

	double stoch[_numeq_];
	sode_system_stoch(t, y, stoch, p, noise);

	for (int i = 0; i < _numeq_; i++)
	{
		yout[i] = y[i] + yout[i] * _m_dt_ + stoch[i];
//		yout[i] = fmod(yout[i], 2*M_PI);
	}
}