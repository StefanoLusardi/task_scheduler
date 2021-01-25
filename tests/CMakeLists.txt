cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(PROJECT_NAME ssts_tests)

include(DownloadGoogleTest)
include(GoogleTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")

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
	src/test_pool.cpp
	# src/test_sanitizer.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE ssts gtest)

gtest_discover_tests(${PROJECT_NAME})
