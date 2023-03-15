cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

file(STRINGS "VERSION" VERSION_STR)
project(ssts 
	LANGUAGES CXX
	VERSION ${VERSION_STR}
	DESCRIPTION "ssTs: Small & Simple Task Scheduler for C++17"
	HOMEPAGE_URL "https://github.com/StefanoLusardi/task_scheduler"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/modules ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_library(ssts INTERFACE)
add_library(ssts::ssts ALIAS ssts)

target_compile_features(ssts INTERFACE cxx_std_17)
target_include_directories(ssts INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
	$<INSTALL_INTERFACE:CMAKE_INSTALL_INCLUDEDIR>)

option(SSTS_ENABLE_CODE_COVERAGE "Enable code coverage (requires installing library)" False)
option(SSTS_BUILD_DOCS "Build library documentation" False)
option(SSTS_BUILD_TESTS "Build library tests" True)
option(SSTS_BUILD_EXAMPLES "Build library examples" True)
option(SSTS_INSTALL_LIBRARY "Install library" False)
option(SSTS_INSTALL_EXAMPLES "Install examples (requires installing library and building examples)" False)
option(SSTS_ENABLE_SANITIZERS "Run unit tests with Thread Sanitizer support" False)

if(${SSTS_ENABLE_SANITIZERS})
	message(STATUS "::ssTs:: Sanitizers enabled")
	if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(ssts INTERFACE -fno-omit-frame-pointer)
        target_compile_options(ssts INTERFACE -g -fsanitize=thread)
        target_link_libraries(ssts INTERFACE -fsanitize=thread)
    elseif(MSVC)
        message(STATUS "Thread Sanitizer not supported on MSVC")
        target_compile_options(ssts INTERFACE -fsanitize=address)
        target_link_libraries(ssts INTERFACE -fsanitize=address)
    endif()
endif()

if(${SSTS_ENABLE_CODE_COVERAGE})
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
		message(STATUS "::ssTs:: Code coverage enabled")
		target_compile_options(ssts INTERFACE --coverage -fPIC -O0 -g)
        target_link_options(ssts INTERFACE --coverage -fPIC)
    elseif(MSVC)
        message(STATUS "Code coverage not supported on MSVC")
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
	enable_testing()
	add_subdirectory(tests)
endif()

if(${SSTS_INSTALL_LIBRARY})
	message(STATUS "::ssTs:: Install library")
	include(GNUInstallDirs)
	install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
	install(TARGETS ssts EXPORT ssts DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()
