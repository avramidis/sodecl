//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//


// StochasticEuler solver
inline static void sode_solver(double t, __global double y[_numeq_], __global double yout[_numeq_], __global double stoch[_numeq_], __global double p[_numpar_], __global double noise[_numnoi_])
{
	int ig = get_global_id(0);
	
	sode_system(t, y, yout, p);

	sode_system_stoch(t, y, stoch, p, noise);

	for (int i = 0; i < _numeq_; i++)
	{
		yout[ig * _numeq_ + i] = y[ig * _numeq_ + i] + yout[ig * _numeq_ + i] * _m_dt_ + stoch[ig * _numeq_ + i];
	}
}