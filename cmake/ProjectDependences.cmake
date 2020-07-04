find_package(OpenCL REQUIRED)
if (${OpenCL_FOUND})
    message(STATUS "OpenCL FOUND")
    message(STATUS "OpenCL include dirs: " ${OpenCL_INCLUDE_DIRS})
    message(STATUS "OpenCL libraries: " ${OpenCL_LIBRARIES})
    message(STATUS "OpenCL version: " ${OpenCL_VERSION_STRING})
else ()
    message(FATAL_ERROR "PYTHON3 NOT FOUND")
endif ()

find_package(Python3 COMPONENTS Development)
if (${Python3_FOUND})
    message(STATUS "PYTHON3 FOUND")
    message(STATUS ${Python3_INCLUDE_DIRS})
    message(STATUS ${Python3_LIBRARIES})
else ()
    message(FATAL_ERROR "PYTHON3 NOT FOUND")
endif ()

include(FetchContent)
FetchContent_Declare(pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG origin/master)
FetchContent_MakeAvailable(pybind11)