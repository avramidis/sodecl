#define NOSC _numeq_

inline static void ode_system(double t, double* y, double* yout, double* p)
{
	// Kuramoto model
	for (int i = 0; i<NOSC; i++)
	{
		yout[i] = p[i];
		//#pragma unroll 1
		for (int j = 0; j<NOSC; j++)
		{
			yout[i] = yout[i] + (sin(y[j] - y[i]));
		}
		yout[i] = yout[i] * (double)(1.00 / (double)(NOSC));
	}

}

inline static void ode_system_stoch(double t, double* y, double* stoch, double* p, double* noise)
{
	//#pragma unroll 1
	for (int i = 0; i < NOSC; i++)
	{
		stoch[i] = p[_numpar_- _numnoi_ + i] * noise[i];
	}
}