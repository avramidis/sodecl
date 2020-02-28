//---------------------------------------------------------------------------//
// Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "cxxplot.hpp"
#include "sodecl.hpp"
#include <iostream>

int
main()
{
    int number_of_orbits = 2;
    int number_of_equations = 6;
    int number_of_parameters = 6;

    std::vector<double> t_0(number_of_orbits, 0);
    std::vector<double> y_0;
    std::vector<double> parameters;

    for (int i = 0; i<number_of_orbits; i++) {
        y_0.push_back(0);
        y_0.push_back(0);
        y_0.push_back(0);
        y_0.push_back(0);
        y_0.push_back(0);
        y_0.push_back(2);
    }

    for (int i = 0; i<number_of_orbits; i++) {
        parameters.push_back(120);
        parameters.push_back(1.5);
        parameters.push_back(0.0045);
        parameters.push_back(0.05);
        parameters.push_back(600);
        parameters.push_back(9);
    }

    sodecl::solver_Type solver_type = sodecl::solver_Type::Euler;

    double dt = 1e-8;
    double time_span = 0.1;
    int number_of_kernel_steps = 4000;
    int number_of_noise = 0;

    sodecl::sodeclmgr* mysodeclmgr = new sodecl::sodeclmgr("kernels",
            "oculomotor.cl",
            solver_type,
            dt,
            time_span,
            number_of_kernel_steps,
            number_of_equations,
            number_of_parameters,
            number_of_noise,
            number_of_orbits,
            sodecl::output_Type::Array);

    // Choose device
    int platform_index = 0;
    int device_index = 0;

    int success = mysodeclmgr->choose_device(platform_index, sodecl::device_Type::ALL, device_index);
    if (success==0) {
        cerr << "Error selecting OpenCL device!" << endl;
    }

    mysodeclmgr->set_t0(t_0.data());
    mysodeclmgr->set_y0(y_0.data());
    mysodeclmgr->set_params(parameters.data());

    // Set the local group size
    int local_group_size = 0;
    mysodeclmgr->set_local_group_size(local_group_size);

    // Setup and run the SODE solver
    int ret = mysodeclmgr->setup_sode_solver();
    if (ret==0) {
        return -1;
    }

    mysodeclmgr->run_sode_solver();

    std::cout << mysodeclmgr->m_output.size() << std::endl;

    std::vector<double> y_out_0;
    for (int i = 0; i<mysodeclmgr->m_output.size(); i += number_of_orbits) {
        for (int j = 0; i<mysodeclmgr->m_output.size(); j += 6) {
            y_out_0.push_back(mysodeclmgr->m_output[i*6+j]);
        }
    }

    //std::vector<double> y_1{5.0, 3.2, 10.0, 12.4, 39.2};
    cxxplot::Plot<double> plot_1(y_out_0);
    plot_1.set_xlabel("x label");
    plot_1.set_ylabel("y label");
    plot_1.show();

    return 0;
}