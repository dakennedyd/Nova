
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
#include "ISingleton.h"
#include "ISubSystem.h"
#include <string>
#include <vector>
#include <windows.h>

struct aiScene;
namespace Nova
{

struct TextureInfo
{
    // void * textureBuffer;
    // unsigned char * textureBuffer;
    // float * textureBufferFP;
    std::vector<void *> textureBuffer;
    int width;
    int height;
    int bpp;
    bool isHDR;
};

class FileSystem : public ISingleton<FileSystem>, public ISubSystem
{
  public:
    FileSystem() = default;
    ~FileSystem() = default;
    void startUp() override;
    void shutDown() override;

    std::string loadFileAsString(const std::string &pathAndFileName);

    /* don't call this function directly if you do you need
    to free the texture afterwards width freeTexture() */
    const TextureInfo loadTexture(const std::string &fileAndPath, const bool uvFlipped);
    const TextureInfo loadTexture(const std::vector<std::string> filesAndPath,
                                  const bool uvFlipped);
    void freeTexture(void *ti);

    const aiScene *loadModel(const std::string &pathAndFileName);

    std::vector<std::string> getFilenamesInDirectory(const std::string &path,
                                                     bool recursive = true);
    std::string getFilenameExtension(const std::string &filenameAndPath);

    bool static fileExists(const std::string &name)
    {
        LPCTSTR szPath = name;
        DWORD dwAttrib = GetFileAttributes(szPath);

        return (dwAttrib != INVALID_FILE_ATTRIBUTES /*&& !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)*/);
    }

    std::string static getExecutablePath()
    {
        char result[MAX_PATH];
        return std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
    }

  private:
};
} // namespace Nova
