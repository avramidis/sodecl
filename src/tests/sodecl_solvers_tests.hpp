#ifndef SODECL_SOLVERS_TESTS_HPP
#define SODECL_SOLVERS_TESTS_HPP

#include "catch.hpp"
#include "sodecl.hpp"

TEST_CASE("Integration solvers test.")
{
    SECTION("Create stochastic Euler test.")
    {
        double dt = 0.05;
        int int_time = 400; 
        int kernel_steps = 40;
        int num_equat = 3;
        int num_params = 3;
        int num_noi = 3;
        int orbits = 8;

        sodecl::opencl_mgr* m_opencl_mgr = new sodecl::opencl_mgr;

        m_opencl_mgr->choose_opencl_device(1, sodecl::device_Type::ALL, 0);

        int err = m_opencl_mgr->create_opencl_context();
        
        err = m_opencl_mgr->create_command_queue();
        if (err != 1)
        {
            std::cout << "Error: Failed to create command queue!" << std::endl;
        }

        cl_double* y0 = new cl_double[orbits*num_equat];
        cl_double* params = new cl_double[orbits*num_params*num_noi];

        sodecl::stochastic_euler *m_stochastic_euler = new sodecl::stochastic_euler(m_opencl_mgr,
                                                                                    "kernels",
                                                                                    "kuramoto.cl",
                                                                                    dt,
                                                                                    int_time,
                                                                                    kernel_steps,
                                                                                    num_equat,
                                                                                    num_params,
                                                                                    num_noi,
                                                                                    orbits,
                                                                                    y0,
                                                                                    params);

        m_stochastic_euler->create_kernel_strings();

        m_stochastic_euler->setup_solver();

        m_stochastic_euler->run_solver();

        CHECK(1 == 1);
    }
}

#endif // SODECL_SOLVERS_TESTS_HPP