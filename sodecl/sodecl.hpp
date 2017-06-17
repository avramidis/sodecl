//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef sodecl_MAINCL_HPP
#define sodecl_MAINCL_HPP

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define _SCL_SECURE_NO_WARNINGS

// TODO: In case I want to define for different architectures
#include <sodecl/cl.hpp>
#ifdef _WIN32
	#include <sodecl/cl.hpp>
#elif __APPLE__
	#include <sodecl/cl.hpp>		// TODO
#endif

#include <sodecl/clog.hpp>
#include <sodecl/timer.hpp>
#include <sodecl/build_Option.hpp>
#include <sodecl/output_Type.hpp>
#include <sodecl/device_Type.hpp>
#include <sodecl/solver_Type.hpp>
#include <sodecl/device.hpp>
#include <sodecl/platform.hpp>
#include <sodecl/system.hpp>

#endif // sodecl_MAINCL_HPP
