#define T1 0.15
#define T2 0.012
#define Tn 25.00

double sburstf(double x, double aval, double bval, double atval, double btval)
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

double dsburstf(double x, double aval, double bval, double atval, double btval)
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

void calc_jacobian(double y[_numeq_], double jac[_numeq_][_numeq_], double p[_numpar_])
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

void sode_system(double t, double y[_numeq_], double yout[_numeq_], double p[_numpar_])
{
	// g
	yout[0] = y[1];
	// u
	yout[1] = -(((1.00 / T1) + (1.00 / T2)) * y[1]) - (1.00 / (T1 * T2) * y[0]) + (1.00 / (T1 * T2) * y[2]) + ((1.00 / T1) + (1.00 / T2)) * (y[3] - y[4]);
	// n
	yout[2] = -(y[2] / Tn) + (y[3] - y[4]);
	//r
	yout[3] = (1.00 / p[2])*(-y[3] - (p[3] * y[3] * y[4] * y[4]) + sburstf(y[5], p[0], p[1], p[4], p[5]));
	// l
	yout[4] = (1.00 / p[2])*(-y[4] - (p[3] * y[4] * y[3] * y[3]) + sburstf(-y[5], p[0], p[1], p[4], p[5]));
	// m
	yout[5] = -(y[3] - y[4]);
}

void sode_system_stoch(double t, double y[_numeq_], double stoch[_numeq_], double p[_numpar_], double noise[_numnoi_])
{
	// g
	stoch[0] = 0;
	// u
	stoch[1] = ((1.00 / T1) + (1.00 / T2)) * (y[3] - y[4])*p[_numpar_ - _numnoi_ + 0] * noise[0] + p[_numpar_ - _numnoi_ + 1] * noise[1];
	// n
    stoch[2] = 0;
	// r
	stoch[3] = 0;
	// l
	stoch[4] = 0;
	// m
	stoch[5] = 0;
}