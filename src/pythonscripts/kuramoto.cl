#define NOSC _numeq_

inline static void sode_system(double t, __global double y[_numeq_], __global double yout[_numeq_], __global double p[_numpar_])
{
	int ig = get_global_id(0);
	int paramstep = ig * _numpar_;
	
	// Kuramoto model
	for (int i = 0; i<NOSC; i++)
	{
		yout[ig * _numeq_ + i] = 0;
		//#pragma unroll 1
		for (int j = 0; j<NOSC; j++)
		{
			yout[ig * _numeq_ + i] = yout[ig * _numeq_ + i] + (sin(y[ig * _numeq_ + j] - y[ig * _numeq_ + i]));
		}
		yout[ig * _numeq_ + i] = yout[ig * _numeq_ + i] * (double)(p[paramstep + 0] / (double)(NOSC));
		yout[ig * _numeq_ + i] = yout[ig * _numeq_ + i] + p[paramstep + i+1];
	}
}

inline static void sode_system_stoch(double t, __global double y[_numeq_], __global double stoch[_numeq_], __global double p[_numpar_], __global double noise[_numnoi_])
{
	int ig = get_global_id(0);

	int paramstep = ig * _numpar_;

	//#pragma unroll 1
	for (int i = 0; i < NOSC; i++)
	{
		stoch[ig * _numeq_ + i] = p[paramstep + (_numpar_ - _numnoi_) + i] * noise[ig * _numnoi_ + i];
		//stoch[ig * _numeq_ + i] = 0;
	}
}