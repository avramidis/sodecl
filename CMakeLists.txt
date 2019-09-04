#---------------------------------------------------------------------------//
# Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
#
# Distributed under The MIT License (MIT)
# See accompanying file LICENSE.txt
#---------------------------------------------------------------------------//

cmake_minimum_required (VERSION 3.13.4)
project(sodecl)

set(CMAKE_CXX_STANDARD 11)

set(default_build_type "Release")

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC ")
	add_compile_options(
			"$<$<CONFIG:RELASE>:/O2>"
			"$<$<CONFIG:DEBUG>:/Od;/Zi>")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	add_compile_options(
			"$<$<CONFIG:RELASE>:-O2>"
			"$<$<CONFIG:DEBUG>:-Wall;-Wextra;-Wpedantic;-Og;-g>")
endif()

add_subdirectory(benchmarks)
enable_testing()
add_subdirectory(tests)
add_subdirectory(interfaces)
add_subdirectory(examples)