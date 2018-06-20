//---------------------------------------------------------------------------//
// Copyright (c) 2015,2018 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef SODECL_MAINCL_HPP
#define SODECL_MAINCL_HPP

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define _SCL_SECURE_NO_WARNINGS

#include <CL/cl.hpp>
#include <sodecl/clog.hpp>
#include <sodecl/timer.hpp>
#include <sodecl/build_Option.hpp>
#include <sodecl/output_Type.hpp>
#include <sodecl/device_Type.hpp>
#include <sodecl/solver_Type.hpp>
#include <sodecl/device.hpp>
#include <sodecl/platform.hpp>
#include <sodecl/sodeclmgr.hpp>
#include <sodecl/opencl_mgr.hpp>
#include <sodecl/opencl_source_mgr.hpp>
#include <sodecl/opencl_source_stochastic_euler.hpp>
#include <sodecl/opencl_source_stochastic_noise.hpp>

#endif // SODECL_MAINCL_HPP
