project(benchmarks)

add_custom_target(benchmarks_dummy ALL
        COMMAND ""
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Dummy target for benchmarks.")


add_custom_command(	TARGET benchmarks_dummy
        POST_BUILD

        # Copy python scripts and OpenCL kernels to the Python interface folder
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/src/pythonscripts"
        "${CMAKE_CURRENT_BINARY_DIR}/python"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/src/sodecl/kernels"
        "${CMAKE_CURRENT_BINARY_DIR}/python/kernels"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/src/include/Random123"
        "${CMAKE_CURRENT_BINARY_DIR}/python/Random123"

        # Copy executables to the MATLAB and Python interfaces folders
        COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:sodecl_bin>"
        "${CMAKE_CURRENT_BINARY_DIR}/python"

        COMMENT "Copying support files to build directory"
        )
