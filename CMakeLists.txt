#---------------------------------------------------------------------------//
# Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
#
# Distributed under The MIT License (MIT)
# See accompanying file LICENSE
#---------------------------------------------------------------------------//

cmake_minimum_required(VERSION 3.15)

project(sodecl
        LANGUAGES CXX
        VERSION 1.0.0
        DESCRIPTION "SODECL is a library of ordinary differential equation (ODE) and stochastic differential equation (SDE) solvers in OpenCL."
        HOMEPAGE_URL "https://github.com/avramidis/sodecl")

include(cmake/ProjectDependencies.cmake)

include(cmake/StandardProjectSettings.cmake)
add_library(sodecl_project_options INTERFACE)
target_compile_features(sodecl_project_options INTERFACE cxx_std_17)

add_library(sodecl_project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(sodecl_project_warnings)

add_subdirectory(src)
include(GenerateExportHeader)
generate_export_header(sodecl)

add_subdirectory(interfaces)
add_subdirectory(benchmarks)
enable_testing()
add_subdirectory(tests)
add_subdirectory(examples)