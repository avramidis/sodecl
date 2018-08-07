#define NOSC _numeq_

inline static void ode_system(double t, __global double y[_numeq_], __global double yout[_numeq_], __global double p[_numpar_])
{
	int ig = get_global_id(0);

	// Kuramoto model
	for (int i = 0; i<NOSC; i++)
	{
		yout[ig * _numeq_ + i] = p[ig * _numpar_ + i+1];
		//#pragma unroll 1
		for (int j = 0; j<NOSC; j++)
		{
			yout[ig * _numeq_ + i] = yout[ig * _numeq_ + i] + (sin(y[ig * _numeq_ + j] - y[ig * _numeq_ + i]));
		}
		yout[ig * _numeq_ + i] = yout[ig * _numeq_ + i] * (double)(p[ig * _numpar_ + 0] / (double)(NOSC));
	}

}

inline static void ode_system_stoch(double t, __global double y[_numeq_], __global double stoch[_numeq_], __global double p[_numpar_], __global double noise[_numnoi_])
{
	int ig = get_global_id(0);

	//#pragma unroll 1
	for (int i = 0; i < NOSC; i++)
	{
		stoch[ig * _numeq_ + i] = p[ig * _numpar_ + _numpar_- _numnoi_ + i] * noise[ig * _numnoi_ + i];
	}
}