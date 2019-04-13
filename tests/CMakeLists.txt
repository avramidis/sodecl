cmake_minimum_required (VERSION 3.13.4)

project(tests)

enable_testing()

set(TESTS_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/sodecl_tests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sodecl_python_tests.py
        ${CMAKE_CURRENT_SOURCE_DIR}/kuramoto.cl
        ${CMAKE_CURRENT_SOURCE_DIR}/oculomotor.cl
        ${CMAKE_CURRENT_SOURCE_DIR}/data/oculomotor_euler.pkl)

set(TESTS_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/sodecl_tests.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/sodecl_python_tests.py
        ${CMAKE_CURRENT_BINARY_DIR}/kuramoto.cl
        ${CMAKE_CURRENT_BINARY_DIR}/oculomotor.cl
        ${CMAKE_CURRENT_BINARY_DIR}/data/oculomotor_euler.pkl)

add_custom_command(OUTPUT ${TESTS_FILES}
        COMMAND ${CMAKE_COMMAND} -E copy ${TESTS_SOURCE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/
        COMMENT "Copy test files"
        VERBATIM)

add_custom_target(tests_copy ALL
        DEPENDS ${TESTS_FILES}
        VERBATIM)

FIND_PACKAGE(Python3 REQUIRED)

add_test(NAME tests_python
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/sodecl_python_tests.py)

add_subdirectory(data)
