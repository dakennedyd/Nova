
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

#include "math/Constants.h"
#include "math/Matrix.h"
#include "math/Quaternion.h"
#include "math/Random.h"
#include "math/Vector.h"
#include "math/miscellaneous.h"

#include "Settings.h"
#include "audio/Audio.h"
#include "resource_manager/ResourceManager.h"

#ifdef NOVA_OPENGL
#    include "graphics/opengl/RendererBackendDeferred.h"
#endif

#include "ECS/DefaultComponents.h"
#include "ECS/DefaultSystems.h"

#include "Application.h"

#ifdef COMPILE_TEST_FUNCTIONS
#    include "tests/Test.h"
#endif