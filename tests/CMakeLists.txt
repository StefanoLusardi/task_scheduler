set(TARGET_NAME ssts_tests)

set(TARGET_SRC
	src/main.cpp
	src/test_at.cpp
	src/test_disable.cpp
	src/test_every.cpp
	src/test_in.cpp
	src/test_missing.cpp
	src/test_remove.cpp
	src/test_stop.cpp
	src/test_duplicated.cpp
	src/test_pool.cpp
	src/scheduler_fixture.hpp
)

find_package(GTest)
include(GoogleTest)
add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME} PRIVATE ${TARGET_SRC})
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)
target_link_libraries(${TARGET_NAME} PRIVATE GTest::GTest PRIVATE ssts::ssts)

gtest_discover_tests(${TARGET_NAME})

set(CMAKE_LINK_WHAT_YOU_USE True)