//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//


// The tridiagonal matrix algorithm (Thomas algorithm)
void solvesystem(double jac[_numeq_][_numeq_], double v[_numeq_], double x[_numeq_])
{
	double a[_numeq_];
	for (int i = _numeq_ - 2; i < _numeq_; i++)
	{
		a[i] = 0;
	}
	for (int i = 0; i < (_numeq_ - 2); i++)
	{
		a[i] = jac[i][i + 1];
	}

	double c[_numeq_];	
	c[_numeq_ - 1] = 0;
	for (int i = 0; i < (_numeq_ - 1); i++)
	{
		c[i] = jac[i + 1][i];
	}

	double b[_numeq_];
	for (int i = 0; i < _numeq_; i++)
	{
		b[i] = jac[i][i];
	}

	double m;
	for (int i = 1; i < _numeq_; i++)
	{
		m = a[i] / b[i - 1];
		b[i] = b[i] - m * c[i - 1];
		v[i] = v[i] - m*v[i - 1];
	}
	x[_numeq_ - 1] = v[_numeq_ - 1] / b[_numeq_ - 1];

	for (int i = _numeq_ - 2; i >= 0; i--)
	{
		x[i] = (v[i] - c[i] * x[i + 1]) / b[i];
	}
}

void newton_f(double dt, double t, double initial[_numeq_], double guess[_numeq_], double yout[_numeq_], double p[_numpar_])
{
	double detterm[_numeq_];
	ode_system(t, guess, detterm, p);

	for (int i = 0; i < _numeq_; i++)
	{
		yout[i] = guess[i] - initial[i] - detterm[i] * dt;
	}
}

void newton(double dt, double t, double initial[_numeq_], double guess[_numeq_], double detterm[_numeq_], double p[_numpar_])
{
	double jac[_numeq_][_numeq_];
	double new_out[_numeq_];
	double multi_out[_numeq_];

	double err1;

	for (int N = 0; N < 10; N++)
	{
		newton_f(dt, t, initial, guess, new_out, p);

		err1 = 0;
		for (int i = 0; i < _numeq_; i++)
		{
			err1 = err1 + fabs(new_out[i]);
		}
		if (err1 < _epsilon1_)
		{
			break;
		}

		calc_jacobian(guess, jac, p);
		for (int i = 0; i < _numeq_; i++)
		{
			for (int j = 0; j < _numeq_; j++)
			{
				jac[i][j] = jac[i][j] * (-1.00)*dt;
			}

			jac[i][i] = jac[i][i] + 1;
		}

		solvesystem(jac, new_out, multi_out);
		for (int i = 0; i < _numeq_; i++)
		{
			guess[i] = guess[i] - multi_out[i];
		}
	}

	for (int i = 0; i < _numeq_; i++)
	{
		detterm[i] = guess[i];
	}
}

/////////////////////////////////////
// Implicit euler solver
inline static void ode_solver(double dt, double t, double y[_numeq_], double yout[_numeq_], double p[_numpar_])
{
	double guess[_numeq_];

	for (int i = 0; i < _numeq_; i++)
	{
		guess[i] = y[i];
	}

	newton(dt, t, y, guess, yout, p);
}
