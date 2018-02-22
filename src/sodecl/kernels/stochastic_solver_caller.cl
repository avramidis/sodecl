//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

__kernel void solver_caller(__global double *t0,
	__global double *y0,
	__global double *params_g,
	__global double *counter_g)
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

	threefry2x64_key_t rk = { { i, 0xf00dcafe } };
	threefry2x64_ctr_t rc = { { 0, 0xdecafbad } };	
	
	threefry2x64_ctr_t rr;

	double u0;
	double u1;
	rk.v[0] = 0;

	int number_of_noise_calls = ceil((float)((float)(_numnoi_) * 0.5));

	for (int it = 0; it < _numsteps_; it++)
	{
		int ncounter = 0;
		for (int in = 0; in < number_of_noise_calls; in++)
		{
			// Generate the noise values
			rc.v[0] = i;
			rc.v[1] = counter_g[i]; // some iteration number change between kernel calls.

			rr = threefry2x64(rc, rk);
			counter_g[i] = counter_g[i] + 1;

			if (ncounter < _numnoi_)
			{
				u0 = u01_open_open_64_53(rr.v[0]);
				u1 = u01_open_open_64_53(rr.v[1]);
				noise[ncounter] = sqrt(-2 * log(u0)) * cos(2 * M_PI * u1)*dtsqrt;
				ncounter = ncounter + 1;
			}
			if (ncounter < _numnoi_)
			{
				noise[ncounter] = sqrt(-2 * log(u0)) * sin(2 * M_PI * u1)*dtsqrt;
				ncounter = ncounter + 1;
			}
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