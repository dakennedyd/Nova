#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

#include "opthelpers.h"


#if defined(_WIN64)
#define SZFMT "%I64u"
#elif defined(_WIN32)
#define SZFMT "%u"
#else
#define SZFMT "%zu"
#endif

#ifdef __GNUC__
#define DECL_FORMAT(x, y, z) __attribute__((format(x, (y), (z))))
#else
#define DECL_FORMAT(x, y, z)
#endif


extern FILE *gLogFile;

constexpr inline const char *CurrentPrefix() noexcept { return ""; }
#if defined(__GNUC__) && !defined(_WIN32)
#define AL_PRINT(T, MSG, ...) fprintf(gLogFile, "AL lib: %s %s%s: " MSG, T, CurrentPrefix(), __FUNCTION__ , ## __VA_ARGS__)
#else
void al_print(const char *type, const char *prefix, const char *func, const char *fmt, ...) DECL_FORMAT(printf, 4,5);
#define AL_PRINT(T, ...) al_print((T), CurrentPrefix(), __FUNCTION__, __VA_ARGS__)
#endif

#ifdef __ANDROID__
#include <android/log.h>
#define LOG_ANDROID(T, MSG, ...) __android_log_print(T, "openal", "AL lib: %s%s: " MSG, CurrentPrefix(), __FUNCTION__ , ## __VA_ARGS__)
#else
#define LOG_ANDROID(T, MSG, ...) ((void)0)
#endif

enum LogLevel {
    NoLog,
    LogError,
    LogWarning,
    LogTrace,
    LogRef
};
extern LogLevel gLogLevel;

#define TRACEREF(...) do {                                                    \
    if(UNLIKELY(gLogLevel >= LogRef))                                         \
        AL_PRINT("(--)", __VA_ARGS__);                                        \
} while(0)

#define TRACE(...) do {                                                       \
    if(UNLIKELY(gLogLevel >= LogTrace))                                       \
        AL_PRINT("(II)", __VA_ARGS__);                                        \
    LOG_ANDROID(ANDROID_LOG_DEBUG, __VA_ARGS__);                              \
} while(0)

#define WARN(...) do {                                                        \
    if(UNLIKELY(gLogLevel >= LogWarning))                                     \
        AL_PRINT("(WW)", __VA_ARGS__);                                        \
    LOG_ANDROID(ANDROID_LOG_WARN, __VA_ARGS__);                               \
} while(0)

#define ERR(...) do {                                                         \
    if(UNLIKELY(gLogLevel >= LogError))                                       \
        AL_PRINT("(EE)", __VA_ARGS__);                                        \
    LOG_ANDROID(ANDROID_LOG_ERROR, __VA_ARGS__);                              \
} while(0)

#endif /* LOGGING_H */
