cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(ssts 
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "ssTs: Small & Simple Task Scheduler for C++17"
	HOMEPAGE_URL "https://github.com/StefanoLusardi/task_scheduler"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_library(ssts INTERFACE)
target_include_directories(ssts INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<INSTALL_INTERFACE:CMAKE_INSTALL_INCLUDEDIR>")

option(SSTS_BUILD_DOCS "Build library documentation" ON)
if(${SSTS_BUILD_DOCS})
    message("::ssTs:: Building docs")
    add_subdirectory(docs)
endif()

option(SSTS_BUILD_EXAMPLES "Build library examples" ON)
if(${SSTS_BUILD_EXAMPLES})
    message(STATUS "Building examples")
    add_subdirectory(examples)
endif()

option(SSTS_BUILD_TESTS "Build library tests" ON)
if(${SSTS_BUILD_TESTS})
    message(STATUS "Building tests")
    enable_testing()
    add_subdirectory(tests)
endif()

option(SSTS_INSTALL_LIBRARY "Install library" ON)
if(${SSTS_INSTALL_LIBRARY})
    message(STATUS "Installing library")
    include(GNUInstallDirs)
    install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(TARGETS ssts EXPORT ssts DESTINATION "${CMAKE_INSTALL_LIBDIR}")
    option(SSTS_INSTALL_EXAMPLES "Install examples (requires installing library)" ON)
endif()
