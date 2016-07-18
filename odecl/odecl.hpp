//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef ODECL_MAINCL_HPP
#define ODECL_MAINCL_HPP

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define _SCL_SECURE_NO_WARNINGS

// TODO: In case I want to define for different architectures
#include <odecl/cl.hpp>
#ifdef _WIN32
	#include <odecl/cl.hpp>
#elif __APPLE__
	#include <odecl/cl.hpp>		// TODO
#endif

#include <odecl/clog.hpp>
#include <odecl/timer.hpp>
#include <odecl/build_Option.hpp>
#include <odecl/output_Type.hpp>
#include <odecl/device_Type.hpp>
#include <odecl/solver_Type.hpp>
#include <odecl/device.hpp>
#include <odecl/platform.hpp>
#include <odecl/system.hpp>

#endif // ODECL_MAINCL_HPP