//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef ODECL_DEVICE_TYPE_HPP
#define ODECL_DEVICE_TYPE_HPP

namespace odecl
{
	enum class device_Type
	{
		ALL = CL_DEVICE_TYPE_ALL,
		CPU = CL_DEVICE_TYPE_CPU,
		GPU = CL_DEVICE_TYPE_GPU,
		ACCELERATOR = CL_DEVICE_TYPE_ACCELERATOR
	};
}


#endif // ODECL_SOLVER_TYPE_HPP