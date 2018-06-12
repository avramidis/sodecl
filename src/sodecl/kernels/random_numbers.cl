//---------------------------------------------------------------------------//
// Copyright (c) 2015,2018 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

__kernel void random_numbers(__global double *t0,
	__global double *noise,
	__global double *counter_g)
{
	int gi = get_global_id(0);
    int i = gi*2;
    double dtsqrt = sqrt(_m_dt_);
	
    threefry2x64_key_t rk = { { i, 0xf00dcafe } };
	threefry2x64_ctr_t rc = { { 0, 0xdecafbad } };	
	threefry2x64_ctr_t rr;

	double u0;
	double u1;
	rk.v[0] = 0;

    // Generate the noise values
    rc.v[0] = gi;
    rc.v[1] = counter_g[gi]; // some iteration number change between kernel calls.

    rr = threefry2x64(rc, rk);
    counter_g[gi] = counter_g[gi] + 1;

    u0 = u01_open_open_64_53(rr.v[0]);
    u1 = u01_open_open_64_53(rr.v[1]);
    noise[i] = sqrt(-2 * log(u0)) * cos(2 * M_PI * u1)*dtsqrt;
    noise[i+1] = sqrt(-2 * log(u0)) * sin(2 * M_PI * u1)*dtsqrt;
}