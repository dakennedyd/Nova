
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
//#include <glad/glad.h>
#include "graphics/ITexture.h"
// #include "PreInit.h"
// #ifdef NOVA_WINDOWS_PLATFORM
// 	#include "windows/FileSystem.h"
// #elif defined  NOVA_LINUX_PLATFORM
// 	#include "linux/FileSystem.h"
// #endif
//#include "Texture.h"
#include <vector>

namespace Nova
{
struct TextureInfo;
class TextureCube : public ITexture
{
  public:
    TextureCube() = delete;
    // TextureCube(const std::array<TextureInfo, 6>& textureInfoArray);
    TextureCube(const std::vector<void *> &textureData, int width, int height, int bitsPerPixel = 0,
                bool isHDR = false);
    TextureCube(const TextureInfo &textureInfo);
    TextureCube(const std::string &fileAndPath);
    ~TextureCube();

    TextureCube(const TextureCube &) = delete;            // copy ctor
    TextureCube(TextureCube &&other);                     // move ctor
    TextureCube &operator=(TextureCube const &) = delete; // copy assignment op
    TextureCube &operator=(TextureCube &&other);          // move assignment op

    void bind() const override;
    void unBind() const override;
    unsigned getTextureID() const { return mTextureID; }
    int getWidth() const { return mWidth; }
    int getHeight() const { return mHeight; }

  private:
    unsigned mTextureID = 0;
    int mWidth;
    int mHeight;
    bool mHasMipmaps = false;
    TextureType mType;
    unsigned char *mTextureBuffer;
};

} // namespace Nova
