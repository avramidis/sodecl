//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

__kernel void solver_caller(__global double *t0,
	__global double *y0,
	__global double *params_g,
	__global double *noise_g)
{
	int i = get_global_id(0);

	double dtsqrt = sqrt(_m_dt_);

	double y[_numeq_];
	double detterm[_numeq_];
	double params[_numpar_];
	double noise[_numnoi_];

	#pragma unroll 1
	for (int ieq = 0; ieq < _numeq_; ieq++)
	{
		y[ieq] = y0[i * _numeq_ + ieq];
	}
	#pragma unroll 1
	for (int ipar = 0; ipar < _numpar_; ipar++)
	{
		params[ipar] = params_g[i * _numpar_ + ipar];
	}

	for (int it = 0; it < _numsteps_; it++)
	{
		for (int in = 0; in < _numnoi_; in++)
		{
			noise[in] = noise_g[it+in+i*_numsteps_*_numnoi_];
		}

		// Call the solver
		ode_solver(t0[i], y, detterm, params, noise);

		t0[i] = t0[i] + _m_dt_;
		#pragma unroll 1
		for (int ieq = 0; ieq < _numeq_; ieq++)
		{
			y[ieq] = detterm[ieq];
		}
	}

	#pragma unroll 1
	for (int ieq = 0; ieq < _numeq_; ieq++)
	{
		y0[i * _numeq_ + ieq] = y[ieq];
	}
}