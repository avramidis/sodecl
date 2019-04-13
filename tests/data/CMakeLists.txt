cmake_minimum_required (VERSION 3.13.4)

project(tests_data_copy)

enable_testing()

set(TESTS_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/oculomotor_euler.pkl)

set(TESTS_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/oculomotor_euler.pkl)

add_custom_command(OUTPUT ${TESTS_FILES}
        COMMAND ${CMAKE_COMMAND} -E copy ${TESTS_SOURCE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/
        COMMENT "Copy test files"
        VERBATIM)

add_custom_target(tests_data_copy ALL
        DEPENDS ${TESTS_FILES}
        VERBATIM)
