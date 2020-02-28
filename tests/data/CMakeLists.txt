enable_testing()

set(TESTS_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/oculomotor_euler.pkl
        ${CMAKE_CURRENT_SOURCE_DIR}/oculomotor_rk.pkl
        ${CMAKE_CURRENT_SOURCE_DIR}/oculomotor_ie.pkl
        ${CMAKE_CURRENT_SOURCE_DIR}/oculomotor_im.pkl)

set(TESTS_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/oculomotor_euler.pkl
        ${CMAKE_CURRENT_BINARY_DIR}/oculomotor_rk.pkl
        ${CMAKE_CURRENT_BINARY_DIR}/oculomotor_ie.pkl
        ${CMAKE_CURRENT_BINARY_DIR}/oculomotor_im.pkl)

add_custom_command(OUTPUT ${TESTS_FILES}
        COMMAND ${CMAKE_COMMAND} -E copy ${TESTS_SOURCE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/
        COMMENT "Copy test files"
        VERBATIM)

add_custom_target(tests_data_copy ALL
        DEPENDS ${TESTS_FILES}
        VERBATIM)
