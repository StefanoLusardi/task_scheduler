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

option(SSTS_ENABLE_CODE_COVERAGE "Enable code coverage (requires installing library)" OFF)
option(SSTS_BUILD_DOCS "Build library documentation" OFF)
option(SSTS_BUILD_TESTS "Build library tests" ON)
option(SSTS_BUILD_EXAMPLES "Build library examples" ON)
option(SSTS_INSTALL_LIBRARY "Install library" OFF)
option(SSTS_INSTALL_EXAMPLES "Install examples (requires installing library and building examples)" OFF)
option(SSTS_ENABLE_SANITIZERS "Run unit tests with Thread Sanitizer support" OFF)

if(${SSTS_ENABLE_SANITIZERS})
	if(NOT WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
		message(STATUS "::ssTs:: Thread Sanitizer enabled")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-omit-frame-pointer -fsanitize=thread")
		set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=thread")
	else()
		message("Thread Sanitizer is not supported on the current platform")
	endif()
endif()

if(${SSTS_ENABLE_CODE_COVERAGE})
	if(NOT WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
		message(STATUS "::ssTs:: Code coverage enabled")
		target_compile_options(ssts INTERFACE --coverage -O0 -g -fno-inline -fno-inline-small-functions -fno-default-inline -fkeep-inline-functions)
		target_link_options(ssts INTERFACE --coverage -fno-inline -fno-inline-small-functions -fno-default-inline -fkeep-inline-functions)
	endif()
endif()  

if(${SSTS_BUILD_DOCS})
	message("::ssTs:: Building docs")
	add_subdirectory(docs)
endif()

if(${SSTS_BUILD_EXAMPLES})
	message(STATUS "::ssTs:: Build examples")
	add_subdirectory(examples)
endif()

if(${SSTS_BUILD_TESTS})
	message(STATUS "::ssTs:: Building tests")
	include(CTest)
	enable_testing()
	add_subdirectory(tests)
endif()

if(${SSTS_INSTALL_LIBRARY})
	message(STATUS "::ssTs:: Install library")
	include(GNUInstallDirs)
	install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
	install(TARGETS ssts EXPORT ssts DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()

if(${SSTS_INSTALL_LIBRARY})
	message(STATUS "::ssTs:: Install examples")
endif()