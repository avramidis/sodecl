//---------------------------------------------------------------------------//
// Copyright (c) 2015,2016 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include <iostream>
#include <chrono>
#include <ctime>

#include "sodecl_export.h"

namespace sodecl {
    class SODECL_EXPORT timer {
    public:
        std::chrono::time_point <std::chrono::system_clock> start, end;

        timer() {
            start = std::chrono::system_clock::now();
        }

        double stop_timer() {
            end = std::chrono::system_clock::now();

            std::chrono::duration<double> elapsed_seconds = end - start;

            //std::time_t end_time = std::chrono::system_clock::to_time_t(end);

            //std::cout << "Elapsed time: " << elapsed_seconds.count() << " sec.\n";

            return elapsed_seconds.count();
        }
    };
}