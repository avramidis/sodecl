add_executable(unittest_opencl sodecl_tests.cpp)
target_include_directories(unittest_opencl PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/sodecl
        ${CMAKE_SOURCE_DIR}/external/include)
target_link_libraries(unittest_opencl PUBLIC OpenCL::OpenCL sodecl_lib)
add_test(NAME unittest_opencl COMMAND unittest_opencl)