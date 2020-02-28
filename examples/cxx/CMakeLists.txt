function(ADD_EXAMPLE EXE_NAME SRC_NAME)
    message(STATUS "Generating ${EXE_NAME} target")

    add_executable(${EXE_NAME})
    target_sources(${EXE_NAME} PRIVATE ${SRC_NAME})

    target_include_directories(${EXE_NAME} PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})
    target_link_libraries(${EXE_NAME} PRIVATE sodecl OpenCL::OpenCL cxxplot Chronos Python3::Python Python3::NumPy)
endfunction()

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/examples/cxx/sodecl.dll
        DEPENDS ${CMAKE_BINARY_DIR}/bin/sodecl.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/sodecl.dll ${CMAKE_BINARY_DIR}/examples/cxx/sodecl.dll
        VERBATIM)
add_custom_target(copy_sodecl_to_examples ALL DEPENDS ${CMAKE_BINARY_DIR}/examples/cxx/sodecl.dll)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/examples/cxx/cxxplot.dll
        DEPENDS ${CMAKE_BINARY_DIR}/bin/cxxplot.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/cxxplot.dll ${CMAKE_BINARY_DIR}/examples/cxx/cxxplot.dll
        VERBATIM)
add_custom_target(copy_cxxplot_to_examples ALL DEPENDS ${CMAKE_BINARY_DIR}/examples/cxx/cxxplot.dll)

add_example(oculomotor_euler oculomotor_euler.cpp)
file(COPY oculomotor.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/src/kernels DESTINATION ${CMAKE_CURRENT_BINARY_DIR})