#ifndef SODECL_OPENLC_KERNEL_STRINGS_TESTS_HPP
#define SODECL_OPENLC_KERNEL_STRINGS_TESTS_HPP

#include "catch.hpp"
#include "sodecl.hpp"


TEST_CASE("OpenCL kernel strings test.")
{
    SECTION("Create noise OpenCL kernel.")
    {
        sodecl::opencl_source_stochastic_noise m_opencl_source_stochastic_noise("kernels", 0.05);
        
        int status_value = m_opencl_source_stochastic_noise.create_kernel_string();
                
        CHECK(status_value == 1);
    }
}

#endif // SODECL_OPENLC_KERNEL_STRINGS_TESTS_HPP