#ifndef AL_COMPAT_H
#define AL_COMPAT_H

#ifdef __cplusplus

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <array>
#include <string>
#include <fstream>

inline std::string wstr_to_utf8(const WCHAR *wstr)
{
    std::string ret;

    int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    if(len > 0)
    {
        ret.resize(len);
        WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &ret[0], len, nullptr, nullptr);
        ret.pop_back();
    }

    return ret;
}

inline std::wstring utf8_to_wstr(const char *str)
{
    std::wstring ret;

    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    if(len > 0)
    {
        ret.resize(len);
        MultiByteToWideChar(CP_UTF8, 0, str, -1, &ret[0], len);
        ret.pop_back();
    }

    return ret;
}


namespace al {

// Windows' std::ifstream fails with non-ANSI paths since the standard only
// specifies names using const char* (or std::string). MSVC has a non-standard
// extension using const wchar_t* (or std::wstring?) to handle Unicode paths,
// but not all Windows compilers support it. So we have to make our own istream
// that accepts UTF-8 paths and forwards to Unicode-aware I/O functions.
class filebuf final : public std::streambuf {
    std::array<char_type,4096> mBuffer;
    HANDLE mFile{INVALID_HANDLE_VALUE};

    int_type underflow() override
    {
        if(mFile != INVALID_HANDLE_VALUE && gptr() == egptr())
        {
            // Read in the next chunk of data, and set the pointers on success
            DWORD got = 0;
            if(ReadFile(mFile, mBuffer.data(), (DWORD)mBuffer.size(), &got, nullptr))
                setg(mBuffer.data(), mBuffer.data(), mBuffer.data()+got);
        }
        if(gptr() == egptr())
            return traits_type::eof();
        return traits_type::to_int_type(*gptr());
    }

    pos_type seekoff(off_type offset, std::ios_base::seekdir whence, std::ios_base::openmode mode) override
    {
        if(mFile == INVALID_HANDLE_VALUE || (mode&std::ios_base::out) || !(mode&std::ios_base::in))
            return traits_type::eof();

        LARGE_INTEGER fpos;
        switch(whence)
        {
            case std::ios_base::beg:
                fpos.QuadPart = offset;
                if(!SetFilePointerEx(mFile, fpos, &fpos, FILE_BEGIN))
                    return traits_type::eof();
                break;

            case std::ios_base::cur:
                // If the offset remains in the current buffer range, just
                // update the pointer.
                if((offset >= 0 && offset < off_type(egptr()-gptr())) ||
                   (offset < 0 && -offset <= off_type(gptr()-eback())))
                {
                    // Get the current file offset to report the correct read
                    // offset.
                    fpos.QuadPart = 0;
                    if(!SetFilePointerEx(mFile, fpos, &fpos, FILE_CURRENT))
                        return traits_type::eof();
                    setg(eback(), gptr()+offset, egptr());
                    return fpos.QuadPart - off_type(egptr()-gptr());
                }
                // Need to offset for the file offset being at egptr() while
                // the requested offset is relative to gptr().
                offset -= off_type(egptr()-gptr());
                fpos.QuadPart = offset;
                if(!SetFilePointerEx(mFile, fpos, &fpos, FILE_CURRENT))
                    return traits_type::eof();
                break;

            case std::ios_base::end:
                fpos.QuadPart = offset;
                if(!SetFilePointerEx(mFile, fpos, &fpos, FILE_END))
                    return traits_type::eof();
                break;

            default:
                return traits_type::eof();
        }
        setg(nullptr, nullptr, nullptr);
        return fpos.QuadPart;
    }

    pos_type seekpos(pos_type pos, std::ios_base::openmode mode) override
    {
        // Simplified version of seekoff
        if(mFile == INVALID_HANDLE_VALUE || (mode&std::ios_base::out) || !(mode&std::ios_base::in))
            return traits_type::eof();

        LARGE_INTEGER fpos;
        fpos.QuadPart = pos;
        if(!SetFilePointerEx(mFile, fpos, &fpos, FILE_BEGIN))
            return traits_type::eof();

        setg(nullptr, nullptr, nullptr);
        return fpos.QuadPart;
    }

public:
    bool open(const wchar_t *filename, std::ios_base::openmode mode)
    {
        if((mode&std::ios_base::out) || !(mode&std::ios_base::in))
            return false;
        HANDLE f{CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ, nullptr,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr)};
        if(f == INVALID_HANDLE_VALUE) return false;

        if(mFile != INVALID_HANDLE_VALUE)
            CloseHandle(mFile);
        mFile = f;

        setg(nullptr, nullptr, nullptr);
        return true;
    }
    bool open(const char *filename, std::ios_base::openmode mode)
    {
        std::wstring wname{utf8_to_wstr(filename)};
        return open(wname.c_str(), mode);
    }

    bool is_open() const noexcept { return mFile != INVALID_HANDLE_VALUE; }

    filebuf() = default;
    ~filebuf() override
    {
        if(mFile != INVALID_HANDLE_VALUE)
            CloseHandle(mFile);
        mFile = INVALID_HANDLE_VALUE;
    }
};

// Inherit from std::istream to use our custom streambuf
class ifstream final : public std::istream {
    filebuf mStreamBuf;

public:
    ifstream(const std::wstring &filename, std::ios_base::openmode mode = std::ios_base::in)
      : ifstream(filename.c_str(), mode) { }
    ifstream(const wchar_t *filename, std::ios_base::openmode mode = std::ios_base::in)
      : std::istream{nullptr}
    {
        init(&mStreamBuf);

        // Set the failbit if the file failed to open.
        if((mode&std::ios_base::out) ||
           !mStreamBuf.open(filename, mode|std::ios_base::in))
            clear(failbit);
    }

    ifstream(const std::string &filename, std::ios_base::openmode mode = std::ios_base::in)
      : ifstream(filename.c_str(), mode) { }
    ifstream(const char *filename, std::ios_base::openmode mode = std::ios_base::in)
      : std::istream{nullptr}
    {
        init(&mStreamBuf);

        // Set the failbit if the file failed to open.
        if((mode&std::ios_base::out) ||
           !mStreamBuf.open(filename, mode|std::ios_base::in))
            clear(failbit);
    }

    bool is_open() const noexcept { return mStreamBuf.is_open(); }
};

} // namespace al

#define HAVE_DYNLOAD 1

#else /* _WIN32 */

#include <fstream>

namespace al {

using filebuf = std::filebuf;
using ifstream = std::ifstream;

} // namespace al

#if defined(HAVE_DLFCN_H)
#define HAVE_DYNLOAD 1
#endif

#endif /* _WIN32 */

#include <string>

struct PathNamePair { std::string path, fname; };
const PathNamePair &GetProcBinary(void);

#ifdef HAVE_DYNLOAD
void *LoadLib(const char *name);
void CloseLib(void *handle);
void *GetSymbol(void *handle, const char *name);
#endif

#endif /* __cplusplus */

#endif /* AL_COMPAT_H */
