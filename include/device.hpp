//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef SODECL_DEVICE_HPP
#define SODECL_DEVICE_HPP

#include <CL/cl.h>
#include "sodecl_export.h"

namespace sodecl {

    class SODECL_EXPORT device {
        /*
        VARIABLES SECTION
        */
    public:

        // Platform ID
        cl_device_id m_device_id;

        // Device ID
        cl_device_id m_device;

        // Device type
        cl_device_type m_device_type;

    private:

        /*
        FUNCTIONS SECTION
        */
    public:

        // Default constructor
        device();

        // Default destructor
        ~device();

        // Costructor with specific platform ID
        device(cl_device_id m_device_id);

        // get device name
        std::string name();

        // get device name
        std::string version();

        // get device type
        std::string type_str();

        cl_device_type type();

        // get platform info based on attribute
        std::string get_info(cl_device_info cl_pi);

    private:

    };

}


#endif // SODECL_DEVICE_HPP
