#ifndef SODECL_SOLVERS_TESTS_HPP
#define SODECL_SOLVERS_TESTS_HPP

#include "catch.hpp"
#include "sodecl.hpp"

TEST_CASE("Integration solvers test.")
{
    SECTION("Create stochastic Euler test.")
    {

        sodecl::opencl_mgr* m_opencl_mgr = new sodecl::opencl_mgr;

        m_opencl_mgr->choose_opencl_device(1, sodecl::device_Type::ALL, 0);

        int err = m_opencl_mgr->create_opencl_context();
        
        err = m_opencl_mgr->create_command_queue();
        if (err != 1)
        {
            std::cout << "Error: Failed to create command queue!" << std::endl;
        }

        sodecl::stochastic_euler *m_stochastic_euler = new sodecl::stochastic_euler(m_opencl_mgr,
                                                                                    "kernels",
                                                                                    "kuramoto.cl",
                                                                                    0.05,
                                                                                    400,
                                                                                    40,
                                                                                    3,
                                                                                    3,
                                                                                    3,
                                                                                    8,
                                                                                    NULL,
                                                                                    NULL);


        m_stochastic_euler->create_kernel_strings();

        m_stochastic_euler->setup_solver();

        m_stochastic_euler->run_solver();

        CHECK(1 == 1);
    }
}

#endif // SODECL_SOLVERS_TESTS_HPP