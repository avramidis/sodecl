cmake_minimum_required(VERSION 3.13.4)

project(interface_python)

FIND_PACKAGE(OpenCL REQUIRED)
FIND_PACKAGE(Python3 REQUIRED)

add_subdirectory(pybind11)
pybind11_add_module(sodecl_python sodecl.cpp)
set_target_properties(sodecl_python PROPERTIES OUTPUT_NAME sodecl_interface)

# Add include directories for OpenCL and sodecl
target_include_directories(sodecl_python PUBLIC ${CMAKE_SOURCE_DIR}/external/include
        ${CMAKE_SOURCE_DIR}/src/sodecl
        ${CMAKE_SOURCE_DIR}/src)

link_directories(${OpenCL_LIBRARY})
target_link_libraries(sodecl_python PUBLIC ${OpenCL_LIBRARIES})
#TARGET_LINK_LIBRARIES(sodecl_python PUBLIC "-lgcov")

get_property(dirs TARGET sodecl_python PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir='${dir}'")
endforeach()

add_custom_command(TARGET sodecl_python POST_BUILD

        # Copy pybind11 generated library to the benchmarks fodler
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sodecl_python> ${CMAKE_BINARY_DIR}/benchmarks/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sodecl.py ${CMAKE_BINARY_DIR}/benchmarks/

        # Copy OpenCL kernels to the benchmarks folder
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/kernels ${CMAKE_BINARY_DIR}/benchmarks//kernels
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/external/include/Random123 ${CMAKE_BINARY_DIR}/benchmarks/Random123

        # Copy pybind11 generated library to the tests fodler
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sodecl_python> ${CMAKE_BINARY_DIR}/tests/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sodecl.py ${CMAKE_BINARY_DIR}/tests/

        # Copy OpenCL kernels to the tests folder
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/kernels ${CMAKE_BINARY_DIR}/tests/kernels
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/external/include/Random123 ${CMAKE_BINARY_DIR}/tests/Random123

        COMMENT "Copying Python interface files"
        )