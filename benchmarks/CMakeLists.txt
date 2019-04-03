project(benchmarks)

set(BENCHMARK_PYTHON_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/kuramoto_benchmark.py
        ${CMAKE_CURRENT_SOURCE_DIR}/kuramoto.cl
        ${CMAKE_CURRENT_SOURCE_DIR}/oculomotor_benchmark.py
        ${CMAKE_CURRENT_SOURCE_DIR}/oculomotor.cl
        )

set(BENCHMARK_PYTHON_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/kuramoto_benchmark.py
        ${CMAKE_CURRENT_BINARY_DIR}/kuramoto.cl
        ${CMAKE_CURRENT_BINARY_DIR}/oculomotor_benchmark.py
        ${CMAKE_CURRENT_BINARY_DIR}/oculomotor.cl
        )

add_custom_command(OUTPUT ${BENCHMARK_PYTHON_FILES}
        COMMAND ${CMAKE_COMMAND} -E copy ${BENCHMARK_PYTHON_SOURCE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/
        COMMENT "Copy benchmark files"
        VERBATIM
        )

add_custom_target(benchmarks_copy ALL
        DEPENDS ${BENCHMARK_PYTHON_FILES}
        VERBATIM)
