
// The MIT License (MIT)

// Copyright (c) 2018 David Kennedy

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

#pragma once

#include "PreInit.h"
#ifdef NOVA_WINDOWS_PLATFORM
#    include <Shlwapi.h>
#    include <cstdio>
#    include <tchar.h>
#    include <windows.h>
#    pragma comment(lib, "shlwapi.lib") // unicode support
#endif

//#include <chrono>
// #include <vector>
// #include <string>
// #include <algorithm>
// #include <fstream>
// #include <array>
// #include <unordered_map>
// #include <functional>
// #include <memory>
// #include <typeindex>
// #include <utility>
// #include <cassert>

// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>

// #include <glad/glad.h>
// #include <GLFW/glfw3.h> // GLFW helper library

// #include "Error.h"
// #include "BasicTypes.h"
// #include "Init.h"
// #include "Crc.h"
// //#include "Pipes.h"
// #include "Timer.h"
// #include "resource_manager/ResourceBase.h"
// #include "ISubSystem.h"
// #include "ISingleton.h"
// #include "logger/Logger.h"

#include "math/Constants.h"
#include "math/Matrix.h"
#include "math/Quaternion.h"
#include "math/Random.h"
#include "math/Vector.h"
#include "math/miscellaneous.h"

// #ifdef NOVA_WINDOWS_PLATFORM
// 	#include "windows/FileSystem.h"
// #elif defined  NOVA_LINUX_PLATFORM
// 	#include "linux/FileSystem.h"
// #endif

#include "Settings.h"
// #include "Xml.h"
#include "resource_manager/ResourceManager.h"

// #include "ECS/Component.h"
// #include "ECS/Entity.h"
// #include "ECS/System.h"

// #ifdef NOVA_WINDOWS_PLATFORM
// 	#include "windows/Window.h" //as im using glfw this header initializes
// opengl 	#include "windows/InputSystem.h" #elif defined NOVA_LINUX_PLATFORM
// 	#include "linux/Window.h"
// 	#include "linux/InputSystem.h"
// #endif

// #include "graphics/ImageDecoder.h"
// #include "graphics/Bindable.h"
// #include "graphics/IRendererFrontend.h"
#ifdef NOVA_OPENGL
//	#include "graphics/opengl/Light.h"
//	#include "graphics/opengl/RendererInit.h"
//	#include "graphics/opengl/Mesh.h"
//	#include "graphics/opengl/GPUProgramParameter.h"
//	#include "graphics/opengl/GPUProgram.h"
//	#include "graphics/opengl/Texture.h"
#    include "graphics/opengl/TextureCube.h"
//	#include "graphics/opengl/FrameBuffer.h"
//	#include "graphics/opengl/Material.h"
//	#include "graphics/opengl/RenderPacket.h"
//	#include "graphics/opengl/RendererBackendForward.h"
#    include "graphics/opengl/RendererBackendDeferred.h"
//	#include "graphics/opengl/GraphicsSystem.h"
//#include "graphics/opengl/Model.h"
#endif

//#include "resource_manager/DefaultLoaders.h"
#include "ECS/DefaultComponents.h"
#include "ECS/DefaultSystems.h"

#include "Application.h"
#include "graphics/RendererFrontend.h"

#ifdef COMPILE_TEST_FUNCTIONS
#    include "tests/Test.h"
#endif