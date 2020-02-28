function(ADD_EXAMPLE EXE_NAME SRC_NAME)
    message(STATUS "Generating ${EXE_NAME} target")

    add_executable(${EXE_NAME})
    target_sources(${EXE_NAME} PRIVATE ${SRC_NAME})

    target_include_directories(${EXE_NAME} PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})
    target_link_libraries(${EXE_NAME} PRIVATE sodecl OpenCL::OpenCL cxxplot Chronos Python3::Python Python3::NumPy)

    set_target_properties(${EXE_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples")
endfunction()


add_example(oculomotor_euler oculomotor_euler.cpp)