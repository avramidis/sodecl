//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

__kernel void solver_caller(__global double *t0,
	__global double *y0,
	__global double *params_g)
{
	int i = get_global_id(0);

	double t;
	double y[_numeq_];
	double detterm[_numeq_];
	double params[_numpar_];

	t = t0[i];

	int k = i * _numeq_;
	for (int m = 0; m < _numeq_; m++)
	{
		y[m] = y0[k + m];
		//printf("%f n \n", y[m]);
	}

	k = i * _numpar_;
	for (int m = 0; m < _numpar_; m++)
	{
		params[m] = params_g[k + m];
		//printf("%f n \n", params[m]);
	}

	int it;
	for (it = 0; it < _numsteps_; it++)
	{
		ode_solver(_m_dt, t, y, detterm, params);

		t = t + _m_dt;
		for (int ieq = 0; ieq < _numeq_; ieq++)
		{
			y[ieq] = detterm[ieq];
		}
		//barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
	}

	t0[i] = t;
	k = i * _numeq_;
	for (int m = 0; m < _numeq_; m++)
	{
		y0[k + m] = y[m];
		//printf("%f n \n", y[m]);
	}
}