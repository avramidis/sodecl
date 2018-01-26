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

# Add include directories for OpenCL and sodecl
include_directories("${PROJECT_BINARY_DIR}/../src/include")
include_directories("${PROJECT_BINARY_DIR}/../src/sodecl")

FIND_PACKAGE(OpenCL REQUIRED)
#INCLUDE_DIRECTORIES(${OpenCL_INCLUDE_DIRS}) 

LINK_DIRECTORIES(${OpenCL_LIBRARY})
TARGET_LINK_LIBRARIES(sodecl ${OpenCL_LIBRARIES})

MESSAGE(STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
MESSAGE(STATUS "CMAKE_CURRENT_SOURCE_DIR : " ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "PROJECT_NAME  : " ${PROJECT_NAME})
MESSAGE(STATUS "CMAKE_COMMAND  : " ${CMAKE_COMMAND})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_SOURCE_DIR}/sodecl/kernels" 
					"${CMAKE_CURRENT_BINARY_DIR}/kernels"
					COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${CMAKE_SOURCE_DIR}/include/Random123" 
					"${CMAKE_CURRENT_BINARY_DIR}/Random123"
					COMMENT "Copying support files to build directory"
					) 
