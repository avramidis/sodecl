#---------------------------------------------------------------------------//
# Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
#
# Distributed under The MIT License (MIT)
# See accompanying file LICENSE.txt
#---------------------------------------------------------------------------//

cmake_minimum_required (VERSION 3.2.2)

set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Select flags.
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

project(sodecl VERSION 0.1.0 LANGUAGES CXX)

# Add all warnings compile options
if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -fprofile-arcs -ftest-coverage)
endif()

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(pybind11)
pybind11_add_module(sodecl_bin "${CMAKE_CURRENT_SOURCE_DIR}/src/sodecl.cpp")

add_executable(sodecl_tests "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/sodecl_tests.cpp")

enable_testing()

set_target_properties(sodecl_bin PROPERTIES OUTPUT_NAME "sodecl_interface")

if (WIN32)
	set_target_properties(sodecl_tests PROPERTIES OUTPUT_NAME "sodecl_tests")
	add_test(sodecl_test ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/tests/sodecl_tests)
else()
	set_target_properties(sodecl_tests PROPERTIES OUTPUT_NAME "sodecl_tests.exe")
	add_test(sodecl_test ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/tests/sodecl_tests.exe)
endif()

# Add include directories for OpenCL and sodecl
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/sodecl")

FIND_PACKAGE(OpenCL REQUIRED)
FIND_PACKAGE(PythonLibs REQUIRED)

include_directories(${PYTHON_INCLUDE_PATH})
MESSAGE(STATUS "PYTHON_INCLUDE_DIRS  : " ${PYTHON_INCLUDE_PATH})
MESSAGE(STATUS "PYTHON_LIBRARIES  : " ${PYTHON_LIBRARIES})

LINK_DIRECTORIES(${OpenCL_LIBRARY})
TARGET_LINK_LIBRARIES(sodecl_bin PRIVATE ${OpenCL_LIBRARIES})
TARGET_LINK_LIBRARIES(sodecl_bin PRIVATE "-lgcov")
TARGET_LINK_LIBRARIES(sodecl_tests ${OpenCL_LIBRARIES})
TARGET_LINK_LIBRARIES(sodecl_tests "-lgcov")

MESSAGE(STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
MESSAGE(STATUS "CMAKE_CURRENT_SOURCE_DIR : " ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "PROJECT_NAME  : " ${PROJECT_NAME})
MESSAGE(STATUS "CMAKE_COMMAND  : " ${CMAKE_COMMAND})
MESSAGE(STATUS "CONFIGURATION  : " ${CMAKE_BUILD_TYPE})

add_custom_command(	TARGET sodecl_bin
					POST_BUILD 
                    # Copy MATLAB scripts and OpenCL kernels to the MATLAB interface folder
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_CURRENT_SOURCE_DIR}/src/matlabscripts"
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/matlabscripts"

					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_CURRENT_SOURCE_DIR}/src/sodecl/kernels" 
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/matlabscripts/kernels"
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_CURRENT_SOURCE_DIR}/src/include/Random123" 
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/matlabscripts/Random123"
					
					# Copy python scripts and OpenCL kernels to the Python interface folder
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_CURRENT_SOURCE_DIR}/src/pythonscripts" 
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/pythonscripts"
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_CURRENT_SOURCE_DIR}/src/sodecl/kernels" 
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/pythonscripts/kernels"
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_CURRENT_SOURCE_DIR}/src/include/Random123" 
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/pythonscripts/Random123"
					
					# Copy executables to the MATLAB and Python interfaces folders
					COMMAND ${CMAKE_COMMAND} -E copy 
					"$<TARGET_FILE:sodecl_bin>"
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/pythonscripts/"

					COMMAND ${CMAKE_COMMAND} -E copy
					"$<TARGET_FILE:sodecl_bin>"
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/matlabscripts/"
					
					COMMENT "Copying support files to build directory"
					) 
	
add_custom_command(TARGET sodecl_tests POST_BUILD 
                    # Copy OpenCL kernels to the build tests folder
                    COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_CURRENT_SOURCE_DIR}/src/sodecl/kernels" 
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/tests/kernels"
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_CURRENT_SOURCE_DIR}/src/include/Random123" 
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/tests/Random123"
                    COMMAND ${CMAKE_COMMAND} -E copy
					"${CMAKE_CURRENT_SOURCE_DIR}/src/tests/kuramoto.cl" 
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/tests/"

                    # Copy test executable to the tests folder
                    COMMAND ${CMAKE_COMMAND} -E copy 
					"$<TARGET_FILE:sodecl_tests>" 
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/tests/"
					COMMAND ${CMAKE_COMMAND} -E copy 
					"${CMAKE_CURRENT_SOURCE_DIR}/src/tests/sodecl_python_tests.py" 
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/tests"
					COMMAND ${CMAKE_COMMAND} -E copy
					"$<TARGET_FILE:sodecl_bin>"
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/tests"
					
					COMMENT "Copying support files to build directory"
) 