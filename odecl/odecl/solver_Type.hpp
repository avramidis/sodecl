//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef odecl_SOLVER_TYPE_HPP
#define odecl_SOLVER_TYPE_HPP

namespace odecl
{
	enum class solver_Type
	{
		StochasticEuler = 0,
		Euler = 1,
		RungeKutta = 2,
		ImplicitEuler = 3,
		ImplicitMidpoint = 4
	};
}

#endif // odecl_SOLVER_TYPE_HPP
