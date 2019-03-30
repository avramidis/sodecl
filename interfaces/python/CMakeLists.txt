find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(../sodecl.i PROPERTIES CPLUSPLUS ON)
include_directories(../src/sodecl)

# Add swig module
SWIG_ADD_LIBRARY(sodecl TYPE STATIC LANGUAGE python SOURCES ../sodecl.i)
swig_link_libraries(sodecl ${PYTHON_LIBRARIES})

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