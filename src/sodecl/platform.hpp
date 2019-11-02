//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef sodecl_PLATFORM_HPP
#define sodecl_PLATFORM_HPP

#include <vector>
#include "../sodecl.hpp"
#include <CL/cl.hpp>

namespace sodecl {
    class platform {
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

        platform() {

        }

        platform(cl_platform_id m_platform_id) {

            // Initialise the clog object
            m_log = clog::getInstance();

            this->m_platform_id = m_platform_id;

            //std::cout << "The platform name is " << name().c_str() << std::endl;
            //std::cout << "The platform version is " << version().c_str() << std::endl;
            m_log->write("The platform name is ");
            m_log->write(name().c_str());
            m_log->write("\n");

            m_log->write("	The platform version is ");
            m_log->write(version().c_str());
            m_log->write("\n");

            m_devices_count = get_device_count();
            if (m_devices_count == 0) {
                throw std::runtime_error("No OpenCL device found.\n");
            }

            if (m_devices_count <= 10) {
                create_devices();
            }

        }

        ~platform() {
            for (auto i : m_devices) {
                delete i;
            }

            m_devices.clear();
        }

        std::vector<sodecl::device *> get_devices() {
            return m_devices;
        }

        // get platform name
        std::string name() {
            return get_info(CL_PLATFORM_NAME);
        }

        // get platform vendor
        std::string vendor() {
            return get_info(CL_PLATFORM_VENDOR);
        }

        // get platform vendor
        std::string version() {
            return get_info(CL_PLATFORM_VERSION);
        }

        // get platform profile
        std::string profile() {
            return get_info(CL_PLATFORM_PROFILE);
        }

        // get platform profile
        std::string extensions() {
            return get_info(CL_PLATFORM_EXTENSIONS);
        }

        // get platform info based on attribute
        std::string get_info(cl_platform_info cl_pi) {
            char *info;
            size_t infoSize;

            // get platform attribute value size
            clGetPlatformInfo(m_platform_id, cl_pi, 0, NULL, &infoSize);
            info = new char[infoSize];

            // get platform attribute value
            clGetPlatformInfo(m_platform_id, cl_pi, infoSize, info, NULL);

            std::string str(info);
            delete info;

            return str;
        }

        // get number of devices
        int get_device_count() {
            cl_uint device_count;

            // get device count
            cl_int err = clGetDeviceIDs(m_platform_id, CL_DEVICE_TYPE_ALL, 0, NULL, &device_count);

            return device_count;
        }

        // Create all sodecl::device objects
        void create_devices() {
            cl_device_id *cpDevice = new cl_device_id[m_devices_count];

            clGetDeviceIDs(m_platform_id, CL_DEVICE_TYPE_ALL, m_devices_count, cpDevice, NULL);

            for (cl_uint i = 0; i < m_devices_count; i++) {
                m_devices.push_back(new device(cpDevice[i]));

                m_log->write("	The device name is ");
                m_log->write(m_devices.back()->name().c_str());
                m_log->write("\n");
            }
            delete[] cpDevice;
        }
    };
}


#endif // sodecl_PLATFORM_HPP
