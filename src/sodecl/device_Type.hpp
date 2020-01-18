//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef sodecl_DEVICE_TYPE_HPP
#define sodecl_DEVICE_TYPE_HPP

namespace sodecl {
    enum class device_Type : uint64_t {
        ALL = CL_DEVICE_TYPE_ALL,
        CPU = CL_DEVICE_TYPE_CPU,
        GPU = CL_DEVICE_TYPE_GPU,
        ACCELERATOR = CL_DEVICE_TYPE_ACCELERATOR
    };
}


#endif // sodecl_SOLVER_TYPE_HPP
