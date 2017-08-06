//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef SODECL_MAINCL_HPP
#define SODECL_MAINCL_HPP

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define _SCL_SECURE_NO_WARNINGS

// TODO: In case I want to define for different architectures
#include <CL/cl.hpp>
//#ifdef _WIN32
//	#include <CL/cl.hpp>
//#elif __APPLE__
//	#include <CL/cl.hpp>		// TODO
//#endif

#include <sodecl/clog.hpp>
#include <sodecl/timer.hpp>
#include <sodecl/build_Option.hpp>
#include <sodecl/output_Type.hpp>
#include <sodecl/device_Type.hpp>
#include <sodecl/solver_Type.hpp>
#include <sodecl/device.hpp>
#include <sodecl/platform.hpp>
#include <sodecl/sodeclmgr.hpp>

#endif // SODECL_MAINCL_HPP
