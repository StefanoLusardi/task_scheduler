cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

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

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    option(SSTS_ENABLE_CODE_COVERAGE "Enable code coverage" ON)
    if(${SSTS_ENABLE_CODE_COVERAGE})
        message(STATUS "::ssTs:: Code coverage enabled")
        target_compile_options(ssts INTERFACE --coverage -O0 -g -fno-inline -fno-inline-small-functions -fno-default-inline -fkeep-inline-functions)
        target_link_options(ssts INTERFACE --coverage -fno-inline -fno-inline-small-functions -fno-default-inline -fkeep-inline-functions)
    endif()
endif()  

option(SSTS_BUILD_DOCS "Build library documentation" OFF)
if(${SSTS_BUILD_DOCS})
    message("::ssTs:: Building docs")
    add_subdirectory(docs)
endif()

option(SSTS_BUILD_EXAMPLES "Build library examples" ON)
if(${SSTS_BUILD_EXAMPLES})
    message(STATUS "::ssTs:: Build examples")
    add_subdirectory(examples)
endif()

option(SSTS_BUILD_TESTS "Build library tests" ON)
if(${SSTS_BUILD_TESTS})
    message(STATUS "::ssTs:: Building tests")
    enable_testing()
    add_subdirectory(tests)
endif()

option(SSTS_INSTALL_LIBRARY "Install library" ON)
if(${SSTS_INSTALL_LIBRARY})
    message(STATUS "::ssTs:: Install library")
    include(GNUInstallDirs)
    install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(TARGETS ssts EXPORT ssts DESTINATION "${CMAKE_INSTALL_LIBDIR}")
    option(SSTS_INSTALL_EXAMPLES "Install examples (requires installing library)" ON)
endif()

option(SSTS_INSTALL_EXAMPLES "Install examples (requires installing library)" ON)
if(${SSTS_INSTALL_LIBRARY})
    message(STATUS "::ssTs:: Install examples")
endif()