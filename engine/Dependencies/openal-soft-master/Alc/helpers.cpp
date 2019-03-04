/**
 * OpenAL cross platform audio library
 * Copyright (C) 2011 by authors.
 * This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * Or go to http://www.gnu.org/copyleft/lgpl.html
 */

#ifdef _WIN32
#ifdef __MINGW32__
#define _WIN32_IE 0x501
#else
#define _WIN32_IE 0x400
#endif
#endif

#include "config.h"

#include <cstdlib>
#include <ctime>
#include <cerrno>
#include <cstdarg>
#include <cctype>
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#ifdef HAVE_PROC_PIDPATH
#include <libproc.h>
#endif

#ifdef __FreeBSD__
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#ifndef AL_NO_UID_DEFS
#if defined(HAVE_GUIDDEF_H) || defined(HAVE_INITGUID_H)
#define INITGUID
#include <windows.h>
#ifdef HAVE_GUIDDEF_H
#include <guiddef.h>
#else
#include <initguid.h>
#endif

DEFINE_GUID(KSDATAFORMAT_SUBTYPE_PCM,        0x00000001, 0x0000, 0x0010, 0x80,0x00, 0x00,0xaa,0x00,0x38,0x9b,0x71);
DEFINE_GUID(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, 0x00000003, 0x0000, 0x0010, 0x80,0x00, 0x00,0xaa,0x00,0x38,0x9b,0x71);

DEFINE_GUID(IID_IDirectSoundNotify,   0xb0210783, 0x89cd, 0x11d0, 0xaf,0x08, 0x00,0xa0,0xc9,0x25,0xcd,0x16);

DEFINE_GUID(CLSID_MMDeviceEnumerator, 0xbcde0395, 0xe52f, 0x467c, 0x8e,0x3d, 0xc4,0x57,0x92,0x91,0x69,0x2e);
DEFINE_GUID(IID_IMMDeviceEnumerator,  0xa95664d2, 0x9614, 0x4f35, 0xa7,0x46, 0xde,0x8d,0xb6,0x36,0x17,0xe6);
DEFINE_GUID(IID_IAudioClient,         0x1cb9ad4c, 0xdbfa, 0x4c32, 0xb1,0x78, 0xc2,0xf5,0x68,0xa7,0x03,0xb2);
DEFINE_GUID(IID_IAudioRenderClient,   0xf294acfc, 0x3146, 0x4483, 0xa7,0xbf, 0xad,0xdc,0xa7,0xc2,0x60,0xe2);
DEFINE_GUID(IID_IAudioCaptureClient,  0xc8adbd64, 0xe71e, 0x48a0, 0xa4,0xde, 0x18,0x5c,0x39,0x5c,0xd3,0x17);

#ifdef HAVE_WASAPI
#include <wtypes.h>
#include <devpropdef.h>
#include <propkeydef.h>
DEFINE_DEVPROPKEY(DEVPKEY_Device_FriendlyName, 0xa45c254e, 0xdf1c, 0x4efd, 0x80,0x20, 0x67,0xd1,0x46,0xa8,0x50,0xe0, 14);
DEFINE_PROPERTYKEY(PKEY_AudioEndpoint_FormFactor, 0x1da5d803, 0xd492, 0x4edd, 0x8c,0x23, 0xe0,0xc0,0xff,0xee,0x7f,0x0e, 0);
DEFINE_PROPERTYKEY(PKEY_AudioEndpoint_GUID, 0x1da5d803, 0xd492, 0x4edd, 0x8c, 0x23,0xe0, 0xc0,0xff,0xee,0x7f,0x0e, 4 );
#endif
#endif
#endif /* AL_NO_UID_DEFS */

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif
#ifdef HAVE_INTRIN_H
#include <intrin.h>
#endif
#ifdef HAVE_CPUID_H
#include <cpuid.h>
#endif
#ifdef HAVE_SYS_SYSCONF_H
#include <sys/sysconf.h>
#endif
#ifdef HAVE_FLOAT_H
#include <cfloat>
#endif
#ifdef HAVE_IEEEFP_H
#include <ieeefp.h>
#endif

#ifndef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#elif defined(_WIN32_IE)
#include <shlobj.h>
#endif

#include <mutex>
#include <vector>
#include <string>
#include <algorithm>

#include "alMain.h"
#include "alu.h"
#include "cpu_caps.h"
#include "fpu_modes.h"
#include "vector.h"
#include "compat.h"
#include "threads.h"


#if defined(HAVE_GCC_GET_CPUID) && (defined(__i386__) || defined(__x86_64__) || \
                                    defined(_M_IX86) || defined(_M_X64))
using reg_type = unsigned int;
static inline void get_cpuid(int f, reg_type *regs)
{ __get_cpuid(f, &regs[0], &regs[1], &regs[2], &regs[3]); }
#define CAN_GET_CPUID
#elif defined(HAVE_CPUID_INTRINSIC) && (defined(__i386__) || defined(__x86_64__) || \
                                        defined(_M_IX86) || defined(_M_X64))
using reg_type = int;
static inline void get_cpuid(int f, reg_type *regs)
{ (__cpuid)(regs, f); }
#define CAN_GET_CPUID
#endif

int CPUCapFlags = 0;

void FillCPUCaps(int capfilter)
{
    int caps = 0;

/* FIXME: We really should get this for all available CPUs in case different
 * CPUs have different caps (is that possible on one machine?). */
#ifdef CAN_GET_CPUID
    union {
        reg_type regs[4];
        char str[sizeof(reg_type[4])];
    } cpuinf[3] = {{ { 0, 0, 0, 0 } }};

    get_cpuid(0, cpuinf[0].regs);
    if(cpuinf[0].regs[0] == 0)
        ERR("Failed to get CPUID\n");
    else
    {
        unsigned int maxfunc = cpuinf[0].regs[0];
        unsigned int maxextfunc;

        get_cpuid(0x80000000, cpuinf[0].regs);
        maxextfunc = cpuinf[0].regs[0];

        TRACE("Detected max CPUID function: 0x%x (ext. 0x%x)\n", maxfunc, maxextfunc);

        TRACE("Vendor ID: \"%.4s%.4s%.4s\"\n", cpuinf[0].str+4, cpuinf[0].str+12, cpuinf[0].str+8);
        if(maxextfunc >= 0x80000004)
        {
            get_cpuid(0x80000002, cpuinf[0].regs);
            get_cpuid(0x80000003, cpuinf[1].regs);
            get_cpuid(0x80000004, cpuinf[2].regs);
            TRACE("Name: \"%.16s%.16s%.16s\"\n", cpuinf[0].str, cpuinf[1].str, cpuinf[2].str);
        }

        if(maxfunc >= 1)
        {
            get_cpuid(1, cpuinf[0].regs);
            if((cpuinf[0].regs[3]&(1<<25)))
                caps |= CPU_CAP_SSE;
            if((caps&CPU_CAP_SSE) && (cpuinf[0].regs[3]&(1<<26)))
                caps |= CPU_CAP_SSE2;
            if((caps&CPU_CAP_SSE2) && (cpuinf[0].regs[2]&(1<<0)))
                caps |= CPU_CAP_SSE3;
            if((caps&CPU_CAP_SSE3) && (cpuinf[0].regs[2]&(1<<19)))
                caps |= CPU_CAP_SSE4_1;
        }
    }
#else
    /* Assume support for whatever's supported if we can't check for it */
#if defined(HAVE_SSE4_1)
#warning "Assuming SSE 4.1 run-time support!"
    caps |= CPU_CAP_SSE | CPU_CAP_SSE2 | CPU_CAP_SSE3 | CPU_CAP_SSE4_1;
#elif defined(HAVE_SSE3)
#warning "Assuming SSE 3 run-time support!"
    caps |= CPU_CAP_SSE | CPU_CAP_SSE2 | CPU_CAP_SSE3;
#elif defined(HAVE_SSE2)
#warning "Assuming SSE 2 run-time support!"
    caps |= CPU_CAP_SSE | CPU_CAP_SSE2;
#elif defined(HAVE_SSE)
#warning "Assuming SSE run-time support!"
    caps |= CPU_CAP_SSE;
#endif
#endif
#ifdef HAVE_NEON
    FILE *file = fopen("/proc/cpuinfo", "rt");
    if(!file)
        ERR("Failed to open /proc/cpuinfo, cannot check for NEON support\n");
    else
    {
        std::string features;
        char buf[256];

        while(fgets(buf, sizeof(buf), file) != nullptr)
        {
            if(strncmp(buf, "Features\t:", 10) != 0)
                continue;

            features = buf+10;
            while(features.back() != '\n')
            {
                if(fgets(buf, sizeof(buf), file) == nullptr)
                    break;
                features += buf;
            }
            break;
        }
        fclose(file);
        file = nullptr;

        if(!features.empty())
        {
            const char *str = features.c_str();
            while(isspace(str[0])) ++str;

            TRACE("Got features string:%s\n", str);
            while((str=strstr(str, "neon")) != nullptr)
            {
                if(isspace(*(str-1)) && (str[4] == 0 || isspace(str[4])))
                {
                    caps |= CPU_CAP_NEON;
                    break;
                }
                ++str;
            }
        }
    }
#endif

    TRACE("Extensions:%s%s%s%s%s%s\n",
        ((capfilter&CPU_CAP_SSE)    ? ((caps&CPU_CAP_SSE)    ? " +SSE"    : " -SSE")    : ""),
        ((capfilter&CPU_CAP_SSE2)   ? ((caps&CPU_CAP_SSE2)   ? " +SSE2"   : " -SSE2")   : ""),
        ((capfilter&CPU_CAP_SSE3)   ? ((caps&CPU_CAP_SSE3)   ? " +SSE3"   : " -SSE3")   : ""),
        ((capfilter&CPU_CAP_SSE4_1) ? ((caps&CPU_CAP_SSE4_1) ? " +SSE4.1" : " -SSE4.1") : ""),
        ((capfilter&CPU_CAP_NEON)   ? ((caps&CPU_CAP_NEON)   ? " +NEON"   : " -NEON")   : ""),
        ((!capfilter) ? " -none-" : "")
    );
    CPUCapFlags = caps & capfilter;
}


FPUCtl::FPUCtl() noexcept
{
#if defined(__GNUC__) && defined(HAVE_SSE)
    if((CPUCapFlags&CPU_CAP_SSE))
    {
        __asm__ __volatile__("stmxcsr %0" : "=m" (*&this->sse_state));
        unsigned int sseState = this->sse_state;
        sseState |= 0x8000; /* set flush-to-zero */
        if((CPUCapFlags&CPU_CAP_SSE2))
            sseState |= 0x0040; /* set denormals-are-zero */
        __asm__ __volatile__("ldmxcsr %0" : : "m" (*&sseState));
    }

#elif defined(HAVE___CONTROL87_2)

    __control87_2(0, 0, &this->state, &this->sse_state);
    _control87(_DN_FLUSH, _MCW_DN);

#elif defined(HAVE__CONTROLFP)

    this->state = _controlfp(0, 0);
    _controlfp(_DN_FLUSH, _MCW_DN);
#endif

    this->in_mode = true;
}

void FPUCtl::leave() noexcept
{
    if(!this->in_mode) return;

#if defined(__GNUC__) && defined(HAVE_SSE)
    if((CPUCapFlags&CPU_CAP_SSE))
        __asm__ __volatile__("ldmxcsr %0" : : "m" (*&this->sse_state));

#elif defined(HAVE___CONTROL87_2)

    unsigned int mode;
    __control87_2(this->state, _MCW_DN, &mode, nullptr);
    __control87_2(this->sse_state, _MCW_DN, nullptr, &mode);

#elif defined(HAVE__CONTROLFP)

    _controlfp(this->state, _MCW_DN);
#endif
    this->in_mode = false;
}


#ifdef _WIN32

const PathNamePair &GetProcBinary()
{
    static PathNamePair ret;
    if(!ret.fname.empty() || !ret.path.empty())
        return ret;

    al::vector<WCHAR> fullpath(256);
    DWORD len;
    while((len=GetModuleFileNameW(nullptr, fullpath.data(), static_cast<DWORD>(fullpath.size()))) == fullpath.size())
        fullpath.resize(fullpath.size() << 1);
    if(len == 0)
    {
        ERR("Failed to get process name: error %lu\n", GetLastError());
        return ret;
    }

    fullpath.resize(len);
    if(fullpath.back() != 0)
        fullpath.push_back(0);

    auto sep = std::find(fullpath.rbegin()+1, fullpath.rend(), '\\');
    sep = std::find(fullpath.rbegin()+1, sep, '/');
    if(sep != fullpath.rend())
    {
        *sep = 0;
        ret.fname = wstr_to_utf8(&*sep + 1);
        ret.path = wstr_to_utf8(fullpath.data());
    }
    else
        ret.fname = wstr_to_utf8(fullpath.data());

    TRACE("Got: %s, %s\n", ret.path.c_str(), ret.fname.c_str());
    return ret;
}


void *LoadLib(const char *name)
{
    std::wstring wname{utf8_to_wstr(name)};
    return LoadLibraryW(wname.c_str());
}
void CloseLib(void *handle)
{ FreeLibrary(static_cast<HMODULE>(handle)); }
void *GetSymbol(void *handle, const char *name)
{
    void *ret{reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(handle), name))};
    if(!ret) ERR("Failed to load %s\n", name);
    return ret;
}


void al_print(const char *type, const char *prefix, const char *func, const char *fmt, ...)
{
    char str[1024];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(str, sizeof(str), fmt, ap);
    va_end(ap);
    str[sizeof(str)-1] = 0;

    std::wstring wstr{utf8_to_wstr(str)};
    fprintf(gLogFile, "AL lib: %s %s%s: %ls", type, prefix, func, wstr.c_str());
    fflush(gLogFile);
}


static inline int is_slash(int c)
{ return (c == '\\' || c == '/'); }

static void DirectorySearch(const char *path, const char *ext, al::vector<std::string> *const results)
{
    std::string pathstr{path};
    pathstr += "\\*";
    pathstr += ext;
    TRACE("Searching %s\n", pathstr.c_str());

    std::wstring wpath{utf8_to_wstr(pathstr.c_str())};
    WIN32_FIND_DATAW fdata;
    HANDLE hdl{FindFirstFileW(wpath.c_str(), &fdata)};
    if(hdl != INVALID_HANDLE_VALUE)
    {
        size_t base = results->size();
        do {
            results->emplace_back();
            std::string &str = results->back();
            str = path;
            str += '\\';
            str += wstr_to_utf8(fdata.cFileName);
            TRACE("Got result %s\n", str.c_str());
        } while(FindNextFileW(hdl, &fdata));
        FindClose(hdl);

        std::sort(results->begin()+base, results->end());
    }
}

al::vector<std::string> SearchDataFiles(const char *ext, const char *subdir)
{
    static std::mutex search_lock;
    std::lock_guard<std::mutex> _{search_lock};

    /* If the path is absolute, use it directly. */
    al::vector<std::string> results;
    if(isalpha(subdir[0]) && subdir[1] == ':' && is_slash(subdir[2]))
    {
        std::string path{subdir};
        std::replace(path.begin(), path.end(), '/', '\\');
        DirectorySearch(path.c_str(), ext, &results);
        return results;
    }
    if(subdir[0] == '\\' && subdir[1] == '\\' && subdir[2] == '?' && subdir[3] == '\\')
    {
        DirectorySearch(subdir, ext, &results);
        return results;
    }

    std::string path;

    /* Search the app-local directory. */
    WCHAR *cwdbuf{_wgetenv(L"ALSOFT_LOCAL_PATH")};
    if(cwdbuf && *cwdbuf != '\0')
    {
        path = wstr_to_utf8(cwdbuf);
        if(is_slash(path.back()))
            path.pop_back();
    }
    else if(!(cwdbuf=_wgetcwd(nullptr, 0)))
        path = ".";
    else
    {
        path = wstr_to_utf8(cwdbuf);
        if(is_slash(path.back()))
            path.pop_back();
        free(cwdbuf);
    }
    std::replace(path.begin(), path.end(), '/', '\\');
    DirectorySearch(path.c_str(), ext, &results);

    /* Search the local and global data dirs. */
    static constexpr int ids[2]{ CSIDL_APPDATA, CSIDL_COMMON_APPDATA };
    for(int id : ids)
    {
        WCHAR buffer[MAX_PATH];
        if(SHGetSpecialFolderPathW(nullptr, buffer, id, FALSE) == FALSE)
            continue;

        path = wstr_to_utf8(buffer);
        if(!is_slash(path.back()))
            path += '\\';
        path += subdir;
        std::replace(path.begin(), path.end(), '/', '\\');

        DirectorySearch(path.c_str(), ext, &results);
    }

    return results;
}

void SetRTPriority(void)
{
    bool failed = false;
    if(RTPrioLevel > 0)
        failed = !SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    if(failed) ERR("Failed to set priority level for thread\n");
}

#else

const PathNamePair &GetProcBinary()
{
    static PathNamePair ret;
    if(!ret.fname.empty() || !ret.path.empty())
        return ret;

    al::vector<char> pathname;
#ifdef __FreeBSD__
    size_t pathlen;
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
    if(sysctl(mib, 4, nullptr, &pathlen, nullptr, 0) == -1)
        WARN("Failed to sysctl kern.proc.pathname: %s\n", strerror(errno));
    else
    {
        pathname.resize(pathlen + 1);
        sysctl(mib, 4, pathname.data(), &pathlen, nullptr, 0);
        pathname.resize(pathlen);
    }
#endif
#ifdef HAVE_PROC_PIDPATH
    if(pathname.empty())
    {
        char procpath[PROC_PIDPATHINFO_MAXSIZE]{};
        const pid_t pid{getpid()};
        if(proc_pidpath(pid, procpath, sizeof(procpath)) < 1)
            ERR("proc_pidpath(%d, ...) failed: %s\n", pid, strerror(errno));
        else
            pathname.insert(pathname.end(), procpath, procpath+strlen(procpath));
    }
#endif
    if(pathname.empty())
    {
        pathname.resize(256);

        const char *selfname{"/proc/self/exe"};
        ssize_t len{readlink(selfname, pathname.data(), pathname.size())};
        if(len == -1 && errno == ENOENT)
        {
            selfname = "/proc/self/file";
            len = readlink(selfname, pathname.data(), pathname.size());
        }
        if(len == -1 && errno == ENOENT)
        {
            selfname = "/proc/curproc/exe";
            len = readlink(selfname, pathname.data(), pathname.size());
        }
        if(len == -1 && errno == ENOENT)
        {
            selfname = "/proc/curproc/file";
            len = readlink(selfname, pathname.data(), pathname.size());
        }

        while(len > 0 && static_cast<size_t>(len) == pathname.size())
        {
            pathname.resize(pathname.size() << 1);
            len = readlink(selfname, pathname.data(), pathname.size());
        }
        if(len <= 0)
        {
            WARN("Failed to readlink %s: %s\n", selfname, strerror(errno));
            return ret;
        }

        pathname.resize(len);
    }
    while(!pathname.empty() && pathname.back() == 0)
        pathname.pop_back();

    auto sep = std::find(pathname.crbegin(), pathname.crend(), '/');
    if(sep != pathname.crend())
    {
        ret.path = std::string(pathname.cbegin(), sep.base()-1);
        ret.fname = std::string(sep.base(), pathname.cend());
    }
    else
        ret.fname = std::string(pathname.cbegin(), pathname.cend());

    TRACE("Got: %s, %s\n", ret.path.c_str(), ret.fname.c_str());
    return ret;
}


#ifdef HAVE_DLFCN_H

void *LoadLib(const char *name)
{
    dlerror();
    void *handle{dlopen(name, RTLD_NOW)};
    const char *err{dlerror()};
    if(err) handle = nullptr;
    return handle;
}
void CloseLib(void *handle)
{ dlclose(handle); }
void *GetSymbol(void *handle, const char *name)
{
    dlerror();
    void *sym{dlsym(handle, name)};
    const char *err{dlerror()};
    if(err)
    {
        WARN("Failed to load %s: %s\n", name, err);
        sym = nullptr;
    }
    return sym;
}

#endif /* HAVE_DLFCN_H */

void al_print(const char *type, const char *prefix, const char *func, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    fprintf(gLogFile, "AL lib: %s %s%s: ", type, prefix, func);
    vfprintf(gLogFile, fmt, ap);
    va_end(ap);

    fflush(gLogFile);
}


static void DirectorySearch(const char *path, const char *ext, al::vector<std::string> *const results)
{
    TRACE("Searching %s for *%s\n", path, ext);
    DIR *dir{opendir(path)};
    if(dir != nullptr)
    {
        const size_t extlen = strlen(ext);
        size_t base = results->size();

        struct dirent *dirent;
        while((dirent=readdir(dir)) != nullptr)
        {
            if(strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0)
                continue;

            size_t len{strlen(dirent->d_name)};
            if(len <= extlen) continue;
            if(strcasecmp(dirent->d_name+len-extlen, ext) != 0)
                continue;

            results->emplace_back();
            std::string &str = results->back();
            str = path;
            if(str.back() != '/')
                str.push_back('/');
            str += dirent->d_name;
            TRACE("Got result %s\n", str.c_str());
        }
        closedir(dir);

        std::sort(results->begin()+base, results->end());
    }
}

al::vector<std::string> SearchDataFiles(const char *ext, const char *subdir)
{
    static std::mutex search_lock;
    std::lock_guard<std::mutex> _{search_lock};

    al::vector<std::string> results;
    if(subdir[0] == '/')
    {
        DirectorySearch(subdir, ext, &results);
        return results;
    }

    /* Search the app-local directory. */
    const char *str{getenv("ALSOFT_LOCAL_PATH")};
    if(str && *str != '\0')
        DirectorySearch(str, ext, &results);
    else
    {
        al::vector<char> cwdbuf(256);
        while(!getcwd(cwdbuf.data(), cwdbuf.size()))
        {
            if(errno != ERANGE)
            {
                cwdbuf.clear();
                break;
            }
            cwdbuf.resize(cwdbuf.size() << 1);
        }
        if(cwdbuf.empty())
            DirectorySearch(".", ext, &results);
        else
        {
            DirectorySearch(cwdbuf.data(), ext, &results);
            cwdbuf.clear();
        }
    }

    // Search local data dir
    if((str=getenv("XDG_DATA_HOME")) != nullptr && str[0] != '\0')
    {
        std::string path{str};
        if(path.back() != '/')
            path += '/';
        path += subdir;
        DirectorySearch(path.c_str(), ext, &results);
    }
    else if((str=getenv("HOME")) != nullptr && str[0] != '\0')
    {
        std::string path{str};
        if(path.back() == '/')
            path.pop_back();
        path += "/.local/share/";
        path += subdir;
        DirectorySearch(path.c_str(), ext, &results);
    }

    // Search global data dirs
    if((str=getenv("XDG_DATA_DIRS")) == nullptr || str[0] == '\0')
        str = "/usr/local/share/:/usr/share/";

    const char *next{str};
    while((str=next) != nullptr && str[0] != '\0')
    {
        next = strchr(str, ':');

        std::string path = (next ? std::string(str, next++) : std::string(str));
        if(path.empty()) continue;

        if(path.back() != '/')
            path += '/';
        path += subdir;

        DirectorySearch(path.c_str(), ext, &results);
    }

    return results;
}

void SetRTPriority()
{
    bool failed = false;
#if defined(HAVE_PTHREAD_SETSCHEDPARAM) && !defined(__OpenBSD__)
    if(RTPrioLevel > 0)
    {
        struct sched_param param;
        /* Use the minimum real-time priority possible for now (on Linux this
         * should be 1 for SCHED_RR) */
        param.sched_priority = sched_get_priority_min(SCHED_RR);
        failed = !!pthread_setschedparam(pthread_self(), SCHED_RR, &param);
    }
#else
    /* Real-time priority not available */
    failed = (RTPrioLevel>0);
#endif
    if(failed)
        ERR("Failed to set priority level for thread\n");
}

#endif
