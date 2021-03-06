cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(PROJECT_NAME ssts_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(add_example EXAMPLE_TARGET)
	add_executable(${EXAMPLE_TARGET} ${EXAMPLE_TARGET}.cpp)
	target_link_libraries(${EXAMPLE_TARGET} PRIVATE ssts)
	if(UNIX)
		target_link_libraries(${EXAMPLE_TARGET} PRIVATE pthread)
	endif()
	
	if(${SSTS_INSTALL_EXAMPLES})
		install(TARGETS ${EXAMPLE_TARGET} RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
	endif()
endfunction()

add_example(example_duplicate)
add_example(example_pool)
add_example(example_test)
add_example(example_recursive)

add_example(example_scheduler)
add_example(example_nested)
add_example(example_bind)
add_example(example_update_interval)