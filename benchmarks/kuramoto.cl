#define NOSC _numeq_

void sode_system(double t, double y[_numeq_], double yout[_numeq_], double p[_numpar_])
{
	for (int i = 0; i<NOSC; i++)
	{
		yout[i] = 0;
		for (int j = 0; j<NOSC; j++)
		{
			yout[i] = yout[i] + (sin(y[j] - y[i]));
		}
		yout[i] = yout[i] * (double)(p[0] / (double)(NOSC));
		yout[i] = yout[i] + p[i+1];
	}
}

void sode_system_stoch(double t, double y[_numeq_], double stoch[_numeq_], double p[_numpar_], double noise[_numnoi_])
{
	for (int i = 0; i < NOSC; i++)
	{
		stoch[i] = p[_numpar_ - _numnoi_ + i] * noise[i];
	}
}