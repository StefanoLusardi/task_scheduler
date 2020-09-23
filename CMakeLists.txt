cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(task_scheduler 
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

add_library(task_scheduler INTERFACE)
target_include_directories(task_scheduler INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<INSTALL_INTERFACE:CMAKE_INSTALL_INCLUDEDIR>")

option(TS_BUILD_EXAMPLES "Build library examples" ON)
if(${TS_BUILD_EXAMPLES})
    message(STATUS "Building examples")
    add_subdirectory(examples)
endif()

option(TS_INSTALL "Install library" ON)
if(${TS_INSTALL})
    message(STATUS "Installing library")
    include(GNUInstallDirs)
    install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(TARGETS task_scheduler EXPORT task_scheduler DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()
