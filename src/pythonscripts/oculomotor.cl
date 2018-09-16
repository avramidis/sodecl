#define T1 0.15
#define T2 0.012
#define Tn 25.00

inline static double sburstf(double x, double aval, double bval, double atval, double btval)
{
	double y;
	y = 0;

	if (x > 0)
	{
		y = (atval * (1.00 - exp(-x / btval)));
    }
	else
	{
		y = (-(aval / bval) * x * exp(x / bval));
	}

	return y;
}

inline static double dsburstf(double x, double aval, double bval, double atval, double btval)
{
	double y;
	y = 0;

	if (x > 0)
	{
		y = (atval*exp(-x / btval)) / btval;
	}
	else
	{
		y = -((aval*exp(x / bval)) / bval + (aval*x*exp(x / bval)) / (bval*bval));
	}
	return y;
}

inline static void calc_jacobian(double y[_numeq_], double jac[_numeq_][_numeq_], double p[_numpar_])
{
	jac[0][0] = 0;
	jac[0][1] = 1;
	jac[0][2] = 0;
	jac[0][3] = 0;
	jac[0][4] = 0;
	jac[0][5] = 0;

	jac[1][0] = -1 / (T1*T2);
	jac[1][1] = -(1 / T1) - (1 / T2);
	jac[1][2] = 1 / (T1*T2);
	jac[1][3] = (1 / T1) + (1 / T2);
	jac[1][4] = -(1 / T1) - (1 / T2);
	jac[1][5] = 0;

	jac[2][0] = 0;
	jac[2][1] = 0;
	jac[2][2] = -1 / Tn;
	jac[2][3] = 1;
	jac[2][4] = -1;
	jac[2][5] = 0;

	jac[3][0] = 0;
	jac[3][1] = 0;
	jac[3][2] = 0;
	jac[3][3] = -(p[3] * (y[4] * y[4]) + 1) / p[2];
	jac[3][4] = -(2 * p[3] * y[4] * y[3]) / p[2];
	jac[3][5] = dsburstf(y[5], p[0], p[1], p[4], p[5]) / p[2];

	jac[4][0] = 0;
	jac[4][1] = 0;
	jac[4][2] = 0;
	jac[4][3] = -(2 * p[3] * y[4] * y[3]) / p[2];
	jac[4][4] = -(p[3] * (y[3] * y[3]) + 1) / p[2];
	jac[4][5] = dsburstf(-y[5], p[0], p[1], p[4], p[5]) / p[2];

	jac[5][0] = 0;
	jac[5][1] = 0;
	jac[5][2] = 0;
	jac[5][3] = -1;
	jac[5][4] = 1;
	jac[5][5] = 0;
}

inline static void sode_system(double t, __global double y[_numeq_], __global double yout[_numeq_], __global double p[_numpar_])
{
	int ig = get_global_id(0);
	int paramstep = ig * _numpar_;

	// g
	yout[ig * _numeq_ + 0] = y[ig * _numeq_ + 1];
	// u
	yout[ig * _numeq_ + 1] = -(((1.00 / T1) + (1.00 / T2)) * y[ig * _numeq_ + 1]) - (1.00 / (T1 * T2) * y[ig * _numeq_ + 0]) + (1.00 / (T1 * T2) * y[ig * _numeq_ + 2]) + ((1.00 / T1) + (1.00 / T2)) * (y[ig * _numeq_ + 3] - y[ig * _numeq_ + 4]);
	// n
	yout[ig * _numeq_ + 2] = -(y[ig * _numeq_ + 2] / Tn) + (y[ig * _numeq_ + 3] - y[ig * _numeq_ + 4]);
	//r
	yout[ig * _numeq_ + 3] = (1.00 / p[paramstep + 2])*(-y[ig * _numeq_ + 3] - (p[paramstep + 3] * y[ig * _numeq_ + 3] * y[ig * _numeq_ + 4] * y[ig * _numeq_ + 4]) + sburstf(y[5], p[paramstep + 0], p[paramstep + 1], p[paramstep + 4], p[paramstep + 5]));
	// l
	yout[ig * _numeq_ + 4] = (1.00 / p[paramstep + 2])*(-y[ig * _numeq_ + 4] - (p[paramstep + 3] * y[ig * _numeq_ + 4] * y[ig * _numeq_ + 3] * y[ig * _numeq_ + 3]) + sburstf(-y[5], p[paramstep + 0], p[paramstep + 1], p[paramstep + 4], p[paramstep + 5]));
	// m
	yout[ig * _numeq_ + 5] = -(y[ig * _numeq_ + 3] - y[ig * _numeq_ + 4]);
}

inline static void sode_system_stoch(double t, __global double y[_numeq_], __global double stoch[_numeq_], __global double p[_numpar_], __global double noise[_numnoi_])
{
	int ig = get_global_id(0);
	int paramstep = ig * _numpar_;

	// g
	stoch[ig * _numeq_ + 0] = 0;
	// u
	stoch[ig * _numeq_ + 1] = ((1.00 / T1) + (1.00 / T2)) * (y[ig * _numeq_ + 3] - y[ig * _numeq_ + 4])*p[_numpar_ - _numnoi_ + 0] * noise[ig * _numnoi_ + 0] + p[_numpar_ - _numnoi_ + 1] * noise[ig * _numnoi_ + 1];	
	// n
    stoch[ig * _numeq_ + 2] = 0;
	// r
	stoch[ig * _numeq_ + 3] = 0;
	// l
	stoch[ig * _numeq_ + 4] = 0;
	// m
	stoch[ig * _numeq_ + 5] = 0;
}