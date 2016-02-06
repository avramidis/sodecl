//---------------------------------------------------------------------------//
// Copyright (c) 2015,2016 Eleftherios Avramidis <el.avramidis@gmail.com>
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE.txt
//---------------------------------------------------------------------------//

#ifndef ODECL_TIMER_HPP
#define ODECL_TIMER_HPP

#include <iostream>
#include <chrono>
#include <ctime>

namespace odecl
{
	class timer
	{
	public:
		std::chrono::time_point<std::chrono::system_clock> start, end;

		timer()
		{
			start = std::chrono::system_clock::now();
		}

		void stop_timer()
		{
			end = std::chrono::system_clock::now();

			std::chrono::duration<double> elapsed_seconds = end - start;
			
			std::time_t end_time = std::chrono::system_clock::to_time_t(end);

			std::cout << "Elapsed time: " << elapsed_seconds.count() << " sec.\n";
		}
	};
}


#endif // ODECL_TIMER_HPP