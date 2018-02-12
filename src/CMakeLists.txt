# 25/01/2018
# MIT License
# author Eleftherios Avramidis

cmake_minimum_required (VERSION 3.5.1)
project(sodecl VERSION 0.1 LANGUAGES CXX)

# Add all warnings compile options
if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set (CMAKE_CXX_STANDARD 11)

add_executable(sodecl sodecl.cpp)
add_executable(sodecl_tests tests/sodecl_tests.cpp)

# Add include directories for OpenCL and sodecl
include_directories("${PROJECT_BINARY_DIR}/../src/include")
include_directories("${PROJECT_BINARY_DIR}/../src/sodecl")

FIND_PACKAGE(OpenCL REQUIRED)
#INCLUDE_DIRECTORIES(${OpenCL_INCLUDE_DIRS}) 

LINK_DIRECTORIES(${OpenCL_LIBRARY})
TARGET_LINK_LIBRARIES(sodecl ${OpenCL_LIBRARIES})
TARGET_LINK_LIBRARIES(sodecl_tests ${OpenCL_LIBRARIES})

MESSAGE(STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
MESSAGE(STATUS "CMAKE_CURRENT_SOURCE_DIR : " ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "PROJECT_NAME  : " ${PROJECT_NAME})
MESSAGE(STATUS "CMAKE_COMMAND  : " ${CMAKE_COMMAND})

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tests")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
                    # Copy MATLAB scripts and OpenCL kernels to the MATLAB interface folder
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_SOURCE_DIR}/matlabscripts" 
					"${CMAKE_CURRENT_BINARY_DIR}/matlabscripts"
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_SOURCE_DIR}/sodecl/kernels" 
					"${CMAKE_CURRENT_BINARY_DIR}/matlabscripts/kernels"
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_SOURCE_DIR}/include/Random123" 
					"${CMAKE_CURRENT_BINARY_DIR}/matlabscripts/Random123"
					
					# Copy python scripts and OpenCL kernels to the Python interface folder
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_SOURCE_DIR}/pythonscripts" 
					"${CMAKE_CURRENT_BINARY_DIR}/pythonscripts"
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_SOURCE_DIR}/sodecl/kernels" 
					"${CMAKE_CURRENT_BINARY_DIR}/pythonscripts/kernels"
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_SOURCE_DIR}/include/Random123" 
					"${CMAKE_CURRENT_BINARY_DIR}/pythonscripts/Random123"
					
					# Copy executables to the MATLAB and Python interfaces folders
					COMMAND ${CMAKE_COMMAND} -E copy 
					"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}*" 
					"${CMAKE_CURRENT_BINARY_DIR}/matlabscripts/"
					COMMAND ${CMAKE_COMMAND} -E copy 
					"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}*" 
					"${CMAKE_CURRENT_BINARY_DIR}/pythonscripts/"

                    # Copy OpenCL kernels to the build tests folder
                    COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_SOURCE_DIR}/sodecl/kernels" 
					"${CMAKE_CURRENT_BINARY_DIR}/tests/kernels"
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_SOURCE_DIR}/include/Random123" 
					"${CMAKE_CURRENT_BINARY_DIR}/tests/Random123"
                    COMMAND ${CMAKE_COMMAND} -E copy
					"${CMAKE_SOURCE_DIR}/tests/kuramoto.cl" 
					"${CMAKE_CURRENT_BINARY_DIR}/tests/"

                    # Copy executables to the tests folder
                    COMMAND ${CMAKE_COMMAND} -E copy 
					"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}*" 
					"${CMAKE_CURRENT_BINARY_DIR}/tests/"

                    # Remove executables from the build folder
                    COMMAND ${CMAKE_COMMAND} -E remove 
					"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}*"
					
					COMMENT "Copying support files to build directory"
					) 
