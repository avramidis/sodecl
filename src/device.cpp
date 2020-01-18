//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include <string>

#include <CL/cl.h>
#include "sodecl_export.h"
#include "device.hpp"

namespace sodecl {


        // Default constructor
        device::device() {

        }

        // Default destructor
        device::~device() {

        }

        // Costructor with specific platform ID
        device::device(cl_device_id m_device_id) {
            this->m_device_id = m_device_id;

            //std::cout << "the name is " << name().c_str() << std::endl;
            //std::cout << "the version is " << version().c_str() << std::endl;
        }

        // get device name
        std::string device::name() {
            return get_info(CL_DEVICE_NAME);
        }

        // get device name
        std::string device::version() {
            return get_info(CL_DEVICE_VERSION);
        }

        // get device type
        std::string device::type_str() {
            return get_info(CL_DEVICE_TYPE);
        }

        cl_device_type device::type() {
            // print device name
            cl_device_type info;

            clGetDeviceInfo(m_device_id, CL_DEVICE_TYPE, sizeof(cl_device_type), &info, NULL);

            return info;
        }

        // get platform info based on attribute
        std::string device::get_info(cl_device_info cl_pi) {
            // print device name
            char *info;
            size_t infoSize;

            clGetDeviceInfo(m_device_id, cl_pi, 0, NULL, &infoSize);

            info = new char[infoSize];
            clGetDeviceInfo(m_device_id, cl_pi, infoSize, info, NULL);

            std::string str(info);
            delete[] info;
            return str;
        }

}