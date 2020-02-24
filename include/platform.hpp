//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef SODECL_PLATFORM_HPP
#define SODECL_PLATFORM_HPP

#include <vector>
#include "sodecl.hpp"

#include "sodecl_export.h"

namespace sodecl {
    class SODECL_EXPORT platform {
        /*
        VARIABLES SECTION
        */
    public:

        // Array with pointer to sodecl::platform objects
        //sodecl::device *m_devices;
        std::vector<sodecl::device *> m_devices;

        // Platform ID
        cl_platform_id m_platform_id;

        // Count of devices
        cl_uint m_devices_count;

        // The num of the selected device in m_devices vector.
        cl_uint m_selected_device;

    private:

        // Log mechanisms
        clog *m_log;                    // Pointer for log.

        /*
        FUNCTIONS SECTION
        */
    public:

        platform();

        platform(cl_platform_id m_platform_id);

        ~platform();

        std::vector<sodecl::device *> get_devices();

        // get platform name
        std::string name();

        // get platform vendor
        std::string vendor();

        // get platform vendor
        std::string version();

        // get platform profile
        std::string profile();

        // get platform profile
        std::string extensions();

        // get platform info based on attribute
        std::string get_info(cl_platform_info cl_pi);

        // get number of devices
        int get_device_count();

        // Create all sodecl::device objects
        void create_devices();
    };
}


#endif // SODECL_PLATFORM_HPP
