cmake_minimum_required(VERSION 3.13)
project(sodecl_test)
add_test(sodecl_test_1 python sodecl_python_tests.py)

add_custom_target(meta_test ALL
        COMMAND ""
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running tests.")

add_custom_command(
        TARGET meta_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/sodecl_python_tests.py
        ${CMAKE_CURRENT_BINARY_DIR}/sodecl_python_tests.py)

#add_custom_command(
#        TARGET meta_test POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#        ${PROJECT_SOURCE_DIR}/*.pkl
#        ${CMAKE_CURRENT_BINARY_DIR})

#add_custom_target(test_cshell
#        COMMAND "python3 entropy_test.py"
#        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#        COMMENT "Running tests.")