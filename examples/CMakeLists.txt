cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(ssts_examples LANGUAGES CXX)

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

add_example(example_pool)
add_example(example_scheduler)