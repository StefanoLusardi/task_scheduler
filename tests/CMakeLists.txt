cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(ssts_tests LANGUAGES CXX)

include(DownloadGoogleTest)
include(GoogleTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
	ssts_tests.cpp
	src/test_at.cpp
	src/test_disable.cpp
	src/test_every.cpp
	src/test_in.cpp
	src/test_missing.cpp
	src/test_remove.cpp
	src/test_stop.cpp
	src/test_duplicated.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE ssts gtest)

gtest_discover_tests(${PROJECT_NAME})
