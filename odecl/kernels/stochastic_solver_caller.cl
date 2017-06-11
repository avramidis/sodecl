//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

__kernel void solver_caller(__global double *t0,
	__global double *y0,
	__global double *params_g,
	__global int *counter_g)
{
	int i = get_global_id(0);

	double t;
	double y[_numeq_];
	double detterm[_numeq_];
	double params[_numpar_];
	double noise[_numnoi_];

	t = t0[i];

	int k = i * _numeq_*_numstepsmulti_;
	for (int m = 0; m < _numeq_; m++)
	{
		y[m] = y0[k + (_numstepsmulti_-1) * _numeq_ + m];
	}

	k = i * _numpar_;
	for (int m = 0; m < _numpar_; m++)
	{
		params[m] = params_g[k + m];
	}

	// Noise generators
	int counter;
	counter = counter_g[i];

	threefry4x64_key_t rk = { { i, 0xf00dcafe, 0xdeadbeef, 0xbeeff00d } };
	threefry4x64_ctr_t rc = { { 0, 0xdecafbad, 0xfacebead, 0x12345678 } };	
	
	threefry4x64_ctr_t rr;

	double u[4];

	rk.v[0] = 0;

	for (int km = 0; km < _numstepsmulti_; ++km)
	{
		int it;
		for (it = 0; it < _numsteps_; it++)
		{
			int number_of_noise_calls = ceil((float)((float)(_numnoi_) * 0.25));
			int ncounter = 0;
			for (int in = 0; in < number_of_noise_calls; in++)
			{
				// Generate the noise values
				rc.v[0] = i;
				rc.v[1] = counter; // some iteration number change between kernel calls.

				rr = threefry4x64(rc, rk);
				counter = counter + 1;

				if (ncounter < _numnoi_)
				{
					u[0] = u01_open_open_64_53(rr.v[0]);
					u[1] = u01_open_open_64_53(rr.v[1]);
					noise[ncounter] = sqrt(-2 * log(u[0])) * cos(2 * M_PI * u[1])*sqrt(_m_dt_);
					ncounter = ncounter + 1;
				}
				if (ncounter < _numnoi_)
				{
					noise[ncounter] = sqrt(-2 * log(u[0])) * sin(2 * M_PI * u[1])*sqrt(_m_dt_);
					ncounter = ncounter + 1;
				}
				if (ncounter < _numnoi_)
				{
					u[2] = u01_open_open_64_53(rr.v[2]);
					u[3] = u01_open_open_64_53(rr.v[3]);
					noise[ncounter] = sqrt(-2 * log(u[2])) * cos(2 * M_PI * u[3])*sqrt(_m_dt_);
					ncounter = ncounter + 1;
				}
				if (ncounter < _numnoi_)
				{
					noise[ncounter] = sqrt(-2 * log(u[2])) * sin(2 * M_PI * u[3])*sqrt(_m_dt_);
					ncounter = ncounter + 1;
				}
			}

			// Call the solver
			ode_solver(_m_dt_, t, y, detterm, params, noise);

			t = t + _m_dt_;
			for (int ieq = 0; ieq < _numeq_; ieq++)
			{
				y[ieq] = detterm[ieq];
			}
		}

		t0[i] = t;
		k = i * _numeq_*_numstepsmulti_;
		for (int m = 0; m < _numeq_; m++)
		{
			y0[k + km * _numeq_ + m] = y[m];
		}
		counter_g[i] = counter;
	}	
}