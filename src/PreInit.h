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

#define GLFW_DLL
#define GLFW_INCLUDE_NONE // lets you include gl extension header in any order
                          // not just before glfw

#ifndef NDEBUG
#    define LOG_ACTIVE // logger is active only on debug build
#endif

#define INFO_LOG_LEVEL_ACTIVE
#define WARNING_LOG_LEVEL_ACTIVE
#define DEBUG_LOG_LEVEL_ACTIVE
#define ERROR_LOG_LEVEL_ACTIVE

//#define STBI_ONLY_JPEG
//#define STBI_ONLY_PNG

#define RIGHT_HAND_COORDS
//#define COMPILE_TEST_FUNCTIONS

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