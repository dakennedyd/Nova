cmake_minimum_required(VERSION 3.0)
project(Nova)

message("")
message("********************* NOVA DEBUG BUILD *********************")
message("")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(GLFW_BUILD_DOCS ON)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Dependencies/glfw)

option(ASSIMP_NO_EXPORT OFF)
option(ASSIMP_BUILD_GLTF_EXPORTER OFF)
option(ASSIMP_BUILD_3DS_EXPORTER OFF)
option(ASSIMP_BUILD_ASSBIN_EXPORTER OFF)
option(ASSIMP_BUILD_ASSXML_EXPORTER OFF)
option(ASSIMP_BUILD_COLLADA_EXPORTER OFF)
option(ASSIMP_BUILD_OBJ_EXPORTER OFF)
option(ASSIMP_BUILD_OPENGEX_EXPORTER OFF)
option(ASSIMP_BUILD_PLY_EXPORTER OFF)
option(ASSIMP_BUILD_FBX_EXPORTER OFF)
option(ASSIMP_BUILD_STL_EXPORTER OFF)
option(ASSIMP_BUILD_X_EXPORTER OFF)
option(ASSIMP_BUILD_X3D_EXPORTER OFF)
option(ASSIMP_BUILD_3MF_EXPORTER OFF)
option(ASSIMP_BUILD_ASSJSON_EXPORTER OFF)
option(ASSIMP_BUILD_STEP_EXPORTER OFF)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_COLLADA_IMPORTER OFF)
option(ASSIMP_BUILD_AMF_IMPORTER OFF)
option(ASSIMP_BUILD_3DS_IMPORTER OFF)
option(ASSIMP_BUILD_AC_IMPORTER OFF)
option(ASSIMP_BUILD_ASE_IMPORTER OFF)
option(ASSIMP_BUILD_ASSBIN_IMPORTER OFF)
option(ASSIMP_BUILD_ASSXML_IMPORTER OFF)
option(ASSIMP_BUILD_B3D_IMPORTER OFF)
option(ASSIMP_BUILD_BVH_IMPORTER OFF)
option(ASSIMP_BUILD_DXF_IMPORTER OFF)
option(ASSIMP_BUILD_CSM_IMPORTER OFF)
option(ASSIMP_BUILD_HMP_IMPORTER OFF)
option(ASSIMP_BUILD_IRRMESH_IMPORTER OFF)
option(ASSIMP_BUILD_IRR_IMPORTER OFF)
option(ASSIMP_BUILD_LWO_IMPORTER OFF)
option(ASSIMP_BUILD_LWS_IMPORTER OFF)
option(ASSIMP_BUILD_MD2_IMPORTER OFF)
option(ASSIMP_BUILD_MD3_IMPORTER OFF)
option(ASSIMP_BUILD_MD5_IMPORTER OFF)
option(ASSIMP_BUILD_MDC_IMPORTER OFF)
option(ASSIMP_BUILD_MDL_IMPORTER OFF)
option(ASSIMP_BUILD_NFF_IMPORTER OFF)
option(ASSIMP_BUILD_NDO_IMPORTER OFF)
option(ASSIMP_BUILD_OFF_IMPORTER OFF)
option(ASSIMP_BUILD_OGRE_IMPORTER OFF)
option(ASSIMP_BUILD_OPENGEX_IMPORTER OFF)
option(ASSIMP_BUILD_PLY_IMPORTER OFF)
option(ASSIMP_BUILD_MS3D_IMPORTER OFF)
option(ASSIMP_BUILD_COB_IMPORTER OFF)
option(ASSIMP_BUILD_BLEND_IMPORTER OFF)
option(ASSIMP_BUILD_IFC_IMPORTER OFF)
option(ASSIMP_BUILD_XGL_IMPORTER OFF)
option(ASSIMP_BUILD_FBX_IMPORTER OFF)
option(ASSIMP_BUILD_Q3D_IMPORTER OFF)
option(ASSIMP_BUILD_Q3BSP_IMPORTER OFF)
option(ASSIMP_BUILD_RAW_IMPORTER OFF)
option(ASSIMP_BUILD_SIB_IMPORTER OFF)
option(ASSIMP_BUILD_SMD_IMPORTER OFF)
option(ASSIMP_BUILD_STL_IMPORTER OFF)
option(ASSIMP_BUILD_TERRAGEN_IMPORTER OFF)
option(ASSIMP_BUILD_3D_IMPORTER OFF)
option(ASSIMP_BUILD_X_IMPORTER OFF)
option(ASSIMP_BUILD_X3D_IMPORTER OFF)
option(ASSIMP_BUILD_GLTF_IMPORTER OFF)
option(ASSIMP_BUILD_3MF_IMPORTER OFF)
option(ASSIMP_BUILD_MMD_IMPORTER OFF)
option(ASSIMP_BUILD_STEP_IMPORTER OFF)
option(ASSIMP_BUILD_STATIC_LIB ON)
option(ASSIMP_ENABLE_BOOST_WORKAROUND ON)
option(ASSIMP_BUILD_SHARED_LIBS OFF)
add_subdirectory(Dependencies/assimp)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(Dependencies/bullet3)

option(ALSOFT_UTILS OFF)
option(ALSOFT_EXAMPLES OFF)
option(ALSOFT_TESTS OFF)
option(ALSOFT_INSTALL OFF)
add_subdirectory(Dependencies/openal-soft)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++14")
    #add_compile_options(/W4 /permissive-)
else()
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wfatal-errors -std=c++14")        
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wfatal-errors -std=c++14")    
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++14 -Wpedantic -Wfatal-errors -msse2")
    
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

file(GLOB DEPENDENCIES_SOURCES Dependencies/glad/src/glad.c                                
                                Dependencies/imgui/*.cpp                               
                                Dependencies/imgui/examples/imgui_impl_glfw.cpp
                                Dependencies/imgui/examples/imgui_impl_opengl3.cpp
                                Dependencies/tinyxml2/tinyxml2.cpp)
                                #Dependencies/glm/glm/glm.hpp)                                
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

add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\"
-D IMGUI_IMPL_OPENGL_LOADER_GLAD)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/Dependencies/openal-soft/)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                ${DEPENDENCIES_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
                    src/
                    ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/assimp/include/
                    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/assimp/include/
                    Dependencies/bullet3/src/
                    Dependencies/
                    Dependencies/glad/include/
                    Dependencies/glfw/include/                    
                    Dependencies/openal-soft/include/
                    Dependencies/imgui/
                    Dependencies/imgui/examples/
                    ${CMAKE_CURRENT_BINARY_DIR}/
                    Dependencies/glm/)                    

include (GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
    #BASE_NAME ${PROJECT_NAME}
    EXPORT_MACRO_NAME NOVA_EXPORT
    EXPORT_FILE_NAME NovaExports.h
    #STATIC_DEFINE NOVA_EXPORT_BUILT_AS_STATIC
    )
                               
target_link_libraries(${PROJECT_NAME} PRIVATE
                                    optimized glfw
                                    optimized assimp
                                    optimized ${GLAD_LIBRARIES}
                                    optimized BulletDynamics
                                    optimized BulletCollision
                                    optimized LinearMath
                                    optimized openal
                                    )
                               

set_target_properties(${PROJECT_NAME} PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
