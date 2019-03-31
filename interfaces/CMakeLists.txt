# Set up swig
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

add_library(${PROJECT_NAME} SHARED sodecl.cpp)
include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/include)
include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/sodecl)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCL_LIBRARIES})

# Add subdirectories for each language if desired
option(BUILD_PYTHON "Build Python SWIG module" ON)
if(BUILD_PYTHON)
    add_subdirectory(python)
endif()