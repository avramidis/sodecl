# 25/01/2018
# MIT License
# author Eleftherios Avramidis

cmake_minimum_required (VERSION 3.6.2)

# Add all warnings compile options
if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(sodecl sodecl.cpp)