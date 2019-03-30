#define NOSC _numeq_

inline static void ode_system(double t, double y[_numeq_], double yout[_numeq_], double p[_numpar_])
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

inline static void ode_system_stoch(double t, double y[_numeq_], double stoch[_numeq_], double p[_numpar_], double noise[_numnoi_])
{
	//#pragma unroll 1
	for (int i = 0; i < NOSC; i++)
	{
		stoch[i] = p[_numpar_- _numnoi_ + i] * noise[i];
	}
}