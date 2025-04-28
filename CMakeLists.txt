cmake_minimum_required(VERSION 3.30)

project(finance-tracker
    DESCRIPTION "Track finance application (PC-only)"
    LANGUAGES CXX
)

include("${PROJECT_SOURCE_DIR}/cmake/compile_options.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/package.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/format.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/linters.cmake")

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

if (CCACHE_FOUND)
    message(STATUS "Using ccache ...")
endif()

enable_formatting()
enable_linters()

add_subdirectory(src)
add_subdirectory(tests)
