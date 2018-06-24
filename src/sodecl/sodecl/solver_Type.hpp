//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef SODECL_SOLVER_TYPE_HPP
#define SODECL_SOLVER_TYPE_HPP

namespace sodecl
{
	enum class solver_type
	{
		STOCHASTICEULER = 0,
		EULER = 1,
		RUNGEKUTTA = 2,
		IMPLICITEULER = 3,
		IMPLICITMIDPOINT = 4
	};
}

#endif // SODECL_SOLVER_TYPE_HPP
