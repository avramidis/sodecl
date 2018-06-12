#ifndef SODECL_OPENLC_TESTS_HPP
#define SODECL_OPENLC_TESTS_HPP

#include "catch.hpp"
#include "sodecl.hpp"


TEST_CASE("Test OpenCL platforms.")
{
    SECTION("Count OpenCL platforms test.")
    {
        sodecl::opencl_mgr m_opencl_mgr;
        int platforms_num = m_opencl_mgr.get_opencl_platform_count();
        
        CHECK(platforms_num != -1);
        if (platforms_num == -1) 
        {
            FAIL("Failed to count the OpenCL platforms! Try to reinstall OpenCL runtime(s) for your device(s).");
        }

        CHECK(platforms_num > 0);
        if (platforms_num == 0) 
        {
            FAIL("No OpenCL platforms! Try to reinstall OpenCL runtime(s) for your device(s).");
        }
    }

    SECTION("Create OpenCL platform objects.")
    {
        sodecl::opencl_mgr m_opencl_mgr;
        int platforms_num = m_opencl_mgr.get_opencl_platform_count();
        int status_value = m_opencl_mgr.create_opencl_platforms();

        CHECK(status_value == 1);
        if (status_value == -1) 
        {
            FAIL("Failed to generate the OpenCL platforms objects! Try to reinstall OpenCL runtime(s) for your device(s).");
        }
    }
}

TEST_CASE("Test choosing the first OpenCL device.")
{
    SECTION("Choose the first OpenCL device of the first OpenCL platform.")
    {
        sodecl::opencl_mgr m_opencl_mgr;
        int platforms_num = m_opencl_mgr.get_opencl_platform_count();
        int status_value = m_opencl_mgr.create_opencl_platforms();

        status_value = m_opencl_mgr.choose_opencl_device(0, sodecl::device_Type::ALL, 0);

        CHECK(status_value == 1);
        if (status_value == -1) 
        {
            FAIL("Failed to choose the OpenCL device selected!");
        }
    }
}

TEST_CASE("Test creation of OpenCL context.")
{
    SECTION("Create OpenCL context.")
    {
        sodecl::opencl_mgr m_opencl_mgr;
        int platforms_num = m_opencl_mgr.get_opencl_platform_count();
        int status_value = m_opencl_mgr.create_opencl_platforms();
        status_value = m_opencl_mgr.choose_opencl_device(0, sodecl::device_Type::ALL, 0);

        status_value = m_opencl_mgr.create_opencl_context();

        CHECK(status_value == 1);
        if (status_value == -1) 
        {
            FAIL("Failed to create the OpenCL context for the selected platform and device!");
        }
    }
}

#endif // SODECL_OPENLC_TESTS_HPP