//---------------------------------------------------------------------------//
// Copyright (c) 2015,2016 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef SODECL_BUILD_OPTION_HPP
#define SODECL_BUILD_OPTION_HPP

#include "sodecl_export.h"

namespace sodecl {
    enum class SODECL_EXPORT build_Option {
        FastRelaxedMath = 1,
        stdCL20 = 2,
        stdCL21 = 3
    };
}


#endif // SODECL_BUILD_OPTION_HPP
