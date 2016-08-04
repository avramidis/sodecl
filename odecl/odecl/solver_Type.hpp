//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef ODECL_SOLVER_TYPE_HPP
#define ODECL_SOLVER_TYPE_HPP

namespace odecl
{
	enum class solver_Type
	{
		Euler = 0,
		RungeKutta = 1,
		ImplicitEuler = 2,
		ImplicitMidpoint = 3
	};
}

#endif // ODECL_SOLVER_TYPE_HPP