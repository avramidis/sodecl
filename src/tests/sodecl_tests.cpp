#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "sodecl.hpp"

#include "opencl_tests.hpp"


// TEST_CASE("Test creation of OpenCL kernel.")
// {
//     SECTION("Create Euler solver kernel.")
//     {
//         sodecl::opencl_mgr m_opencl_mgr;
//         int platforms_num = m_opencl_mgr.get_opencl_platform_count();
//         int status_value = m_opencl_mgr.create_opencl_platforms();
//         status_value = m_opencl_mgr.choose_opencl_device(0, sodecl::device_Type::ALL, 0);
//         status_value = m_opencl_mgr.create_context();

//         int number_of_orbits = 10;
//         int number_of_states = 4;
//         int number_of_params = 6;
//         cl_double *y0 = new cl_double[number_of_orbits*number_of_states];
//         for (int i=0; i<number_of_orbits*number_of_states; ++i)
//         {
//             y0[i]=0;
//         }

//         cl_double *params = new cl_double[number_of_orbits*number_of_params];
//         for (int i=0; i<number_of_orbits*number_of_params; ++i)
//         {
//             params[i]=0;
//         }
        
//         sodecl::euler m_euler("kernels",
//                               "kuramoto.cl",
//                               0.05,
//                               200,
//                               1,
//                               number_of_states,
//                               number_of_params,
//                               1,
//                               sodecl::output_Type::File,
//                               y0,
//                               params);

//         status_value = m_euler.create_kernel_string();

//         CHECK(status_value == 1);
//         if (status_value == -1) 
//         {
//             FAIL("Failed to create the OpenCL kernel string for the Euler solver!");
//         }
//     }
// }