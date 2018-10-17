
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

namespace Nova
{
#ifdef _WIN32
#    define NOVA_WINDOWS_PLATFORM // compile for windows platform
#    ifdef _MSC_VER
#        define _CRT_SECURE_NO_WARNINGS // no warnings on scanf and the like
#    endif
#elif defined __linux__
#    define NOVA_LINUX_PLATFORM
#else
#    define NOVA_PLATFORM_UNKOWN
#endif

//#define GLFW_DLL
#define GLFW_INCLUDE_NONE // lets you include gl extension header in any order
                          // not just before glfw

// #ifndef NDEBUG
// #endif
#define LOG_ACTIVE // logger is active only on debug build

#define INFO_LOG_LEVEL_ACTIVE
#define WARNING_LOG_LEVEL_ACTIVE
#define DEBUG_LOG_LEVEL_ACTIVE
#define ERROR_LOG_LEVEL_ACTIVE

#define RIGHT_HAND_COORDS
#define COMPILE_TEST_FUNCTIONS

#define NOVA_OPENGL // just opengl for now

#ifndef NDEBUG
#    define NOVA_ASSERT(expr)                                                                      \
        if (expr)                                                                                  \
        {                                                                                          \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            LOG_ERROR("Assertion failed: " << #expr << " " << __FILE__ << " "                      \
                                           << " " << __LINE__);                                    \
            /*Engine::shutdown(); */                                                               \
            exit(1);                                                                               \
        }
#else
#    define NOVA_ASSERT(expr) // evaluates to nothing
#endif

} // namespace Nova