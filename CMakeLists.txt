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
    #add_compile_options(-j8)    
    if(LINUX)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

#target_include_directories(${PROJECT_NAME} src/
include_directories(src/
                    ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/assimp/include/
                    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/assimp/include/
                    #${CMAKE_HOME_DIRECTORY}/Dependencies/assimp/include/
                    Dependencies/bullet/src/
                    Dependencies/
                    Dependencies/glad/include/
                    Dependencies/glfw/include/
                    Dependencies/stb/)

# file(GLOB PROJECT_SOURCES 
#                           src/Error.cpp
#                           src/graphics/ImageDecoder.cpp
#                           src/linux/FileSystem.cpp  #fix this for windows!!
#                           src/Timer.cpp
#                           src/logger/Logger.cpp
#                           src/Settings.cpp
#                           src/linux/Window.cpp  #fix this for windows!!
#                           src/linux/InputSystem.cpp
#                           src/graphics/opengl/RendererInit.cpp
#                           src/graphics/opengl/GraphicsSystem.cpp
#                           src/resource_manager/ResourceManager.cpp
#                           src/math/Quaternion.cpp
#                           src/math/Matrix.cpp
#                           src/math/Vector.cpp
#                           src/Main.cpp)
file(GLOB DEPENDENCIES_SOURCES Dependencies/glad/src/glad.c)
# file(GLOB PROJECT_HEADERS src/*.hpp src/*.h)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.c)
# set (EXCLUDE_DIR "/windows/")
# file (GLOB_RECURSE PROJECT_SOURCES "*.cpp" "*.c")
# foreach (TMP_PATH ${SOURCE_FILES})
#     string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
#     if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
#         list (REMOVE_ITEM ${SOURCE_FILES} ${TMP_PATH})
#     endif ()
# endforeach(TMP_PATH)
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

#link_directories(/home/david/src/project_nova/engine/Dependencies/glfw
#                 /home/david/src/project_nova/engine/Dependencies/assimp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${DEPENDENCIES_SOURCES})
                               
target_link_libraries(${PROJECT_NAME} glfw assimp #dl
                               ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
#                      BulletDynamics BulletCollision LinearMath)
                               

#find_library(GLFW3_LOCATION /home/david/src/project_nova/engine/Dependencies/glfw)
#target_link_libraries(${PROJECT_NAME} glfw3 ${GLFW3_LOCATION})

#target_link_libraries(${PROJECT_NAME} assimp glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})

#target_link_libraries(${PROJECT_NAME} glfw3 ${CMAKE_SOURCE_DIR}/Dependencies/glfw/libglfw3.a)
#target_link_libraries(${PROJECT_NAME} assimp ${CMAKE_SOURCE_DIR}/Dependencies/assimp/libassimp.a)

#ADD_LIBRARY(glfw3 STATIC IMPORTED)
#SET_TARGET_PROPERTIES(glfw3 PROPERTIES
#    IMPORTED_LOCATION /home/david/src/project_nova/engine/Dependencies/glfw/libglfw3.a)
#TARGET_LINK_LIBRARIES(${PROJECT_NAME} glfw3)


set_target_properties(${PROJECT_NAME} PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
