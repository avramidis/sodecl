#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "sodecl.hpp"

TEST_CASE("Query OpenCL platforms")
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