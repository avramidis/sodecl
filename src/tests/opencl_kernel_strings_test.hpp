#ifndef SODECL_OPENLC_KERNEL_STRINGS_TESTS_HPP
#define SODECL_OPENLC_KERNEL_STRINGS_TESTS_HPP

#include "catch.hpp"
#include "sodecl.hpp"


TEST_CASE("OpenCL kernel strings test.")
{
    SECTION("Create noise OpenCL kernel.")
    {
        sodecl::opencl_source_stochastic_noise m_opencl_source_stochastic_noise("kernels", 0.05);
        
        // m_opencl_source_stochastic_noise.create_kernel_string();
                
        CHECK(1 == 1);
    }

    // SECTION("Create stochastic Euler OpenCL kernel.")
    // {
    //     sodecl::opencl_source_stochastic_euler m_opencl_source_stochastic_euler("kernels", 0.05, "kuramoto.cl", 40, 3, 4, 5);
        
    //     m_opencl_source_stochastic_euler.create_kernel_string();
                
    //     CHECK(1 == 1);
    // }
}

#endif // SODECL_OPENLC_KERNEL_STRINGS_TESTS_HPP