#---------------------------------------------------------------------------//
# Copyright (c) 2015 Eleftherios Avramidis <el.avramidis@gmail.com>
#
# Distributed under The MIT License (MIT)
# See accompanying file LICENSE
#---------------------------------------------------------------------------//

include(GNUInstallDirs)
add_library(sodecl SHARED)

target_sources(sodecl PRIVATE
        ${CMAKE_SOURCE_DIR}/src/clog.cpp
        ${CMAKE_SOURCE_DIR}/src/device.cpp
        ${CMAKE_SOURCE_DIR}/src/platform.cpp
        ${CMAKE_SOURCE_DIR}/src/sodeclmgr.cpp
        ${CMAKE_SOURCE_DIR}/src/timer.cpp)

target_include_directories(sodecl PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

FIND_PACKAGE(OpenCL REQUIRED)
target_link_libraries(sodecl PRIVATE OpenCL::OpenCL)

set_target_properties(sodecl PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_definitions(sodecl PUBLIC cxxplot_EXPORT)

    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/examples/sodecl.dll
            COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:sodecl>" "$<TARGET_FILE_DIR:plot_1>"
            VERBATIM)
    add_custom_target(copyddlexamples ALL
            DEPENDS ${CMAKE_BINARY_DIR}/examples/sodecl.dll sodecl
            VERBATIM)
endif ()

install(TARGETS sodecl EXPORT sodeclTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT sodecl_RunTime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT sodecl_RunTime NAMELINK_COMPONENT cxxplot_Development
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT sodecl_Development
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        TYPE INCLUDE
        FILES_MATCHING
        PATTERN *.hpp
        PERMISSIONS OWNER_READ OWNER_WRITE
        GROUP_READ
        WORLD_READ)

install(FILES ${CMAKE_BINARY_DIR}/sodecl_export.h
        TYPE INCLUDE
        PERMISSIONS OWNER_READ OWNER_WRITE
        GROUP_READ
        WORLD_READ)

install(EXPORT sodeclTargets
        NAMESPACE sodecl::
        FILE cxxplotConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sodecl)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        cxxplotConfigVersion.cmake
        VERSION 1.0.0
        COMPATIBILITY SameMajorVersion)

install(FILES ${PROJECT_BINARY_DIR}/source/sodeclConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sodecl)