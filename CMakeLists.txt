cmake_minimum_required(VERSION 3.17)

cmake_policy(SET CMP0048 NEW)

macro(auto_enable_feature opt_name feature_name)
    if (${opt_name})
        if (NOT DEFINED VCPKG_MANIFEST_FEATURES)
            message(STATUS "Auto append features: ${feature_name}")
            set(VCPKG_MANIFEST_FEATURES ${feature_name})
        else ()
            list(FIND VCPKG_MANIFEST_FEATURES ${opt_name} index)
            if (index EQUAL -1)
                message(STATUS "Auto append features: ${feature_name}")
                list(APPEND VCPKG_MANIFEST_FEATURES ${feature_name})
            endif ()
        endif ()
    endif ()
endmacro()

option(SESE_ARCHIVE_BUILD_TEST "build unit test targets" OFF)

auto_enable_feature(SESE_ARCHIVE_BUILD_TEST "tests")

project(sese-archive VERSION 0.1)

file(GLOB_RECURSE SRC "${CMAKE_CURRENT_LIST_DIR}/src/sese/archive/*.cpp")

add_library(Archive ${SRC})

if (${MSVC})
    target_compile_options(Archive PRIVATE "/utf-8")
endif ()

find_package(LibArchive REQUIRED)
target_link_libraries(Archive PRIVATE LibArchive::LibArchive)

find_package(sese CONFIG REQUIRED)
target_link_libraries(Archive PUBLIC Sese::Core)

set_target_properties(Archive
        PROPERTIES
        CXX_STANDARD 17
        OUTPUT_NAME "sese.archive"
        PREFIX ""
        WINDOWS_EXPORT_ALL_SYMBOLS ON
)

if (${SESE_ARCHIVE_BUILD_TEST})
    enable_testing()
    file(GLOB_RECURSE TEST_SRC "${CMAKE_CURRENT_LIST_DIR}/test/*.cpp")
    add_executable(AllTestInMain ${TEST_SRC})

    find_package(GTest CONFIG REQUIRED)
    target_link_libraries(AllTestInMain PRIVATE GTest::gtest Archive)
endif ()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/cmake/sese-archive-config.cmake.in
        ${PROJECT_BINARY_DIR}/sese-archive-config.cmake
        INSTALL_DESTINATION lib/cmake/sese-archive
)

install(
        TARGETS Archive
        EXPORT SeseArchiveTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include
)

install(
        DIRECTORY "${PROJECT_SOURCE_DIR}/sese"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING
        PATTERN "*.h"
)

install(
        FILES "${PROJECT_BINARY_DIR}/sese-archive-config.cmake"
        DESTINATION lib/cmake/sese-archive
)

install(
        FILES "${PROJECT_BINARY_DIR}/sese-archive-config.cmake"
        DESTINATION debug/lib/cmake/sese-archive
)

install(
        EXPORT SeseArchiveTargets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sese-archive
        NAMESPACE Sese::
)