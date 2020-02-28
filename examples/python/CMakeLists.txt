set(EXAMPLE_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/oculomotor_euler.py
        ${CMAKE_CURRENT_SOURCE_DIR}/oculomotor_euler_stochastic.py
        ${CMAKE_CURRENT_SOURCE_DIR}/oculomotor_rk.py
        ${CMAKE_CURRENT_SOURCE_DIR}/oculomotor_ie.py
        ${CMAKE_CURRENT_SOURCE_DIR}/oculomotor_im.py
        ${CMAKE_CURRENT_SOURCE_DIR}/oculomotor.cl
        ${CMAKE_CURRENT_SOURCE_DIR}/kuramoto_euler.py
        ${CMAKE_CURRENT_SOURCE_DIR}/kuramoto_euler_stochastic.py
        ${CMAKE_CURRENT_SOURCE_DIR}/kuramoto.cl)

set(EXAMPLE_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/oculomotor_euler.py
        ${CMAKE_CURRENT_BINARY_DIR}/oculomotor_euler_stochastic.py
        ${CMAKE_CURRENT_BINARY_DIR}/oculomotor_rk.py
        ${CMAKE_CURRENT_BINARY_DIR}/oculomotor_ie.py
        ${CMAKE_CURRENT_BINARY_DIR}/oculomotor_im.py
        ${CMAKE_CURRENT_BINARY_DIR}/oculomotor.cl
        ${CMAKE_CURRENT_BINARY_DIR}/kuramoto_euler.py
        ${CMAKE_CURRENT_BINARY_DIR}/kuramoto_euler_stochastic.py
        ${CMAKE_CURRENT_BINARY_DIR}/kuramoto.cl)

add_custom_command(OUTPUT ${EXAMPLE_FILES}
        COMMAND ${CMAKE_COMMAND} -E copy ${EXAMPLE_SOURCE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/
        COMMENT "Copy example files"
        VERBATIM)

add_custom_target(examples_copy ALL
        DEPENDS ${EXAMPLE_FILES}
        VERBATIM)