# 25/01/2018
# MIT License
# author Eleftherios Avramidis

cmake_minimum_required (VERSION 3.2.2)

# Set a default build type for single-configuration
# CMake generators if no build type is set.
IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

# Select flags.
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

project(sodecl VERSION 0.1 LANGUAGES CXX)

# Add all warnings compile options
if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set (CMAKE_CXX_STANDARD 11)

add_executable(sodecl_bin "${CMAKE_CURRENT_SOURCE_DIR}/src/sodecl.cpp")
add_executable(sodecl_tests "${CMAKE_CURRENT_SOURCE_DIR}/src/tests/sodecl_tests.cpp")

if (MSVC)
	set_target_properties(sodecl_bin PROPERTIES OUTPUT_NAME "sodecl")
	set_target_properties(sodecl_tests PROPERTIES OUTPUT_NAME "sodecl_tests")
else()
	set_target_properties(sodecl_bin PROPERTIES OUTPUT_NAME "sodecl.exe")
	set_target_properties(sodecl_tests PROPERTIES OUTPUT_NAME "sodecl_tests.exe")
endif()

enable_testing()
add_test(sodecl_test ${PROJECT_BINARY_DIR}/tests/sodecl_tests)

# Add include directories for OpenCL and sodecl
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/sodecl")

FIND_PACKAGE(OpenCL REQUIRED)
#INCLUDE_DIRECTORIES(${OpenCL_INCLUDE_DIRS}) 

LINK_DIRECTORIES(${OpenCL_LIBRARY})
TARGET_LINK_LIBRARIES(sodecl_bin ${OpenCL_LIBRARIES})
TARGET_LINK_LIBRARIES(sodecl_tests ${OpenCL_LIBRARIES})

MESSAGE(STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
MESSAGE(STATUS "CMAKE_CURRENT_SOURCE_DIR : " ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "PROJECT_NAME  : " ${PROJECT_NAME})
MESSAGE(STATUS "CMAKE_COMMAND  : " ${CMAKE_COMMAND})
MESSAGE(STATUS "PROJECT_NAME  : " ${PROJECT_NAME})
 

add_custom_command(	TARGET sodecl_bin
					POST_BUILD 
                    # Copy MATLAB scripts and OpenCL kernels to the MATLAB interface folder
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					${CMAKE_CURRENT_SOURCE_DIR}/src/matlabscripts
					${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/matlabscripts

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
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/sodecl.exe" 
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/matlabscripts/"
					COMMAND ${CMAKE_COMMAND} -E copy 
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/sodecl.exe" 
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/pythonscripts/"

                    # Remove executables from the build folder
                    # COMMAND ${CMAKE_COMMAND} -E remove 
					# "${CMAKE_CURRENT_BINARY_DIR}/sodecl"
					
					COMMENT "Copying support files to build directory"
					) 

#file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tests")

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
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/sodecl_tests.exe" 
					"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/tests/"

                    # COMMAND ${CMAKE_COMMAND} -E copy 
					# "${CMAKE_CURRENT_BINARY_DIR}/src/sodecl_tests" 
					# "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/tests/"

                    # Remove executables from the build folder
                    # COMMAND ${CMAKE_COMMAND} -E remove 
					# "${CMAKE_CURRENT_BINARY_DIR}/sodecl_tests"
					
					COMMENT "Copying support files to build directory"
					) 
