# 25/01/2018
# MIT License
# author Eleftherios Avramidis

cmake_minimum_required (VERSION 3.6.2)
project(sodecl VERSION 0.1 LANGUAGES CXX)

# Add all warnings compile options
if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(sodecl sodecl.cpp)

# Add include directories for OpenCL and sodecl
include_directories("${PROJECT_BINARY_DIR}/../src/include")
include_directories("${PROJECT_BINARY_DIR}/../src/sodecl")

FIND_PACKAGE(OpenCL REQUIRED)
#INCLUDE_DIRECTORIES(${OpenCL_INCLUDE_DIRS}) 

LINK_DIRECTORIES(${OpenCL_LIBRARY})
TARGET_LINK_LIBRARIES(sodecl ${OpenCL_LIBRARIES})

