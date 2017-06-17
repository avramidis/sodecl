//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef odecl_DEVICE_TYPE_HPP
#define odecl_DEVICE_TYPE_HPP

namespace odecl
{
	enum class device_Type : uint64_t
	{
		ALL = CL_DEVICE_TYPE_ALL,
		CPU = CL_DEVICE_TYPE_CPU,
		GPU = CL_DEVICE_TYPE_GPU,
		ACCELERATOR = CL_DEVICE_TYPE_ACCELERATOR
	};
}


#endif // odecl_SOLVER_TYPE_HPP
