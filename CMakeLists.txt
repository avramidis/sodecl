#---------------------------------------------------------------------------//
# Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
#
# Distributed under The MIT License (MIT)
# See accompanying file LICENSE.txt
#---------------------------------------------------------------------------//

cmake_minimum_required (VERSION 3.2.2)
project(sodecl)

set(CMAKE_CXX_STANDARD 11)

set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Add all warnings compile options
if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(
		"$<$<CONFIG:RELASE>:-Wall;-O2>"
		"$<$<CONFIG:DEBUG>:-Wall;-Wextra;-Wpedantic;-O0;-g>"
	)
	#add_compile_options(-Wall -Wextra -Wpedantic -fprofile-arcs -ftest-coverage -fprofile-dir="${CMAKE_CURRENT_SOURCE_DIR}/src/sodecl")
endif()

add_subdirectory(benchmarks)
add_subdirectory(tests)
add_subdirectory(interfaces)