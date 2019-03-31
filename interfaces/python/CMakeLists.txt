project(swig_python)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(../sodecl.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(../sodecl.i PROPERTIES SWIG_FLAGS "-includeall")
include_directories(../src/sodecl)

# Add swig module
swig_add_library(pysodecl TYPE MODULE LANGUAGE python SOURCES ../sodecl.i)
target_include_directories(${SWIG_MODULE_pysodecl_REAL_NAME} PUBLIC ${PYTHON_INCLUDE_DIRS})
swig_link_libraries(${SWIG_MODULE_pysodecl_REAL_NAME} PUBLIC sodecl ${PYTHON_LIBRARIES})

add_custom_target(python_interface ALL COMMAND "")

add_custom_command(
        TARGET ${SWIG_MODULE_pysodecl_REAL_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/interfaces/python/_pysodecl.so
        ${CMAKE_BINARY_DIR}/tests/_pysodecl.so)

add_custom_command(
        TARGET ${SWIG_MODULE_pysodecl_REAL_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/interfaces/python/pysodecl.py
        ${CMAKE_BINARY_DIR}/tests/pysodecl.py)

## Files to install with Python
#set(PYTHON_INSTALL_FILES
#        ${CMAKE_CURRENT_BINARY_DIR}/swig_example.py
#        ${CMAKE_CURRENT_BINARY_DIR}/_swig_example.so)
#
## Configure setup.py and copy to output directory
#set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
#set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
#configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})
#
## Declare install target for python
##install(TARGETS swig_example
##        COMMAND "${PYTHON_EXECUTABLE} setup.py"
##        COMPONENT swig-python)
#
## Install target to call setup.py
#add_custom_target(install-python
#        DEPENDS _swig_example
#        COMMAND python ${SETUP_PY_OUT} install)