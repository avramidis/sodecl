#---------------------------------------------------------------------------//
# Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
#
# Distributed under The MIT License (MIT)
# See accompanying file LICENSE
#---------------------------------------------------------------------------//

cmake_minimum_required (VERSION 3.15)

project(sodecl CXX)

# Project dependences
include(cmake/ProjectDependences.cmake)

# Project settings
include(cmake/StandardProjectSettings.cmake)
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

# Standard compiler warnings
add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

add_subdirectory(src)
include(GenerateExportHeader)
generate_export_header(sodecl)

add_subdirectory(interfaces)
add_subdirectory(benchmarks)
enable_testing()
add_subdirectory(tests)
add_subdirectory(examples)