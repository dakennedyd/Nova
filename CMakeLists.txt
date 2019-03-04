cmake_minimum_required(VERSION 3.0)
project(example)
message("********************* BUILDING NOVA EXAMPLE *********************")
add_subdirectory(engine/)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++14")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++14 -Wpedantic -Wfatal-errors -msse2")        
endif()

file(GLOB_RECURSE PROJECT_SOURCES example/*.cpp example/*.c)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
                               
target_link_libraries(${PROJECT_NAME} Nova)
                               

set_target_properties(${PROJECT_NAME} PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
