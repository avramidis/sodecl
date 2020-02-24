//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef SODECL_SOLVER_TYPE_HPP
#define SODECL_SOLVER_TYPE_HPP

#include "sodecl_export.h"

namespace sodecl {
    enum class SODECL_EXPORT solver_Type {
        StochasticEuler = 0,
        Euler = 1,
        RungeKutta = 2,
        ImplicitEuler = 3,
        ImplicitMidpoint = 4
    };
}

#endif // SODECL_SOLVER_TYPE_HPP
