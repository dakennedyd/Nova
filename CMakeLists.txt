cmake_minimum_required(VERSION 3.0)
project(Nova)

option(GLFW_BUILD_DOCS ON)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Dependencies/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(Dependencies/assimp)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
#add_subdirectory(Dependencies/bullet)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
    #add_compile_options(/W4 /permissive-)
else()
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    #add_compile_options(-Wall -Wextra -Wpedantic -Wfatal-errors -std=c++14)    
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wfatal-errors -std=c++14")    
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors -std=c++14")    
    #if(LINUX)
        #set(GLAD_LIBRARIES dl)
    #endif()
endif()

include_directories(src/
                    ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/assimp/include/
                    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/assimp/include/
                    Dependencies/bullet/src/
                    Dependencies/
                    Dependencies/glad/include/
                    Dependencies/glfw/include/
                    Dependencies/stb/)

file(GLOB DEPENDENCIES_SOURCES Dependencies/glad/src/glad.c)
# file(GLOB PROJECT_HEADERS src/*.hpp src/*.h)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.c)

if(WIN32)
    list(REMOVE_ITEM PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/Window.cpp
                                     ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/FileSystem.cpp
                                     ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/InputSystem.cpp)
else()
    list(REMOVE_ITEM PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/Window.cpp
                                     ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/FileSystem.cpp
                                     ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/InputSystem.cpp)
endif()

# file(GLOB PROJECT_SHADERS Resources/Shaders/glsl/*.glsl
#                           Resources/Shaders/glsl/*.frag
#                           Resources/Shaders/glsl/*.geom
#                           Resources/Shaders/glsl/*.vert)
# file(GLOB PROJECT_CONFIGS CMakeLists.txt
#                           Readme.md
#                          .gitattributes
#                          .gitignore
#                          .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Dependencies" FILES ${DEPENDENCIES_SOURCES})

add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${DEPENDENCIES_SOURCES})
                               
target_link_libraries(${PROJECT_NAME} glfw assimp
                               ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
#                      BulletDynamics BulletCollision LinearMath)
                               

set_target_properties(${PROJECT_NAME} PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
