
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
#include "graphics/ITexture.h"
#ifdef NOVA_WINDOWS_PLATFORM
#    include "windows/FileSystem.h"
#elif defined NOVA_LINUX_PLATFORM
#    include "linux/FileSystem.h"
#endif
#include <glad/glad.h>

namespace Nova
{
class Texture : public ITexture
{
    friend class FrameBuffer;

  public:
    Texture() = delete;
    // filtering option only is relevant for color textures(COLOR, COLOR_ALPHA) the rest are all
    // NEAREST
    Texture(const int width, const int height, const void *textureData = nullptr,
            const enum TextureType &textureType = TextureType::COLOR,
            const enum Filtering &filtering = Filtering::LINEAR,
            const enum MipMapping &mipmap = MipMapping::GENERATE_MIPMAP);
    Texture(const std::string &fileAndPath);

    ~Texture();
    Texture(const Texture &) = delete;            // copy ctor
    Texture(Texture &&other);                     // move ctor
    Texture &operator=(Texture const &) = delete; // copy assignment op
    Texture &operator=(Texture &&other);          // move assignment op

    void bind() const override;
    void unBind() const override;

    // binds and unbinds the texture
    void setFiltering(const enum Filtering &option);
    unsigned getTextureID() const { return mTextureID; }
    TextureType getTextureType() const { return mType; }

    static Texture makeShadowMap(const int width, const int height)
    {
        return Texture(width, height, nullptr, TextureType::DEPTHBUFFER);
    }

    int getWidth() { return mWidth; }
    int getHeight() { return mHeight; }

  private:
    Texture(const TextureInfo &textureInfo);
    unsigned mTextureID; // opengl texture handle
    float mBorderColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    // TextureState mState;
    int mWidth;
    int mHeight;
    TextureType mType;
    bool mHasMipmaps = false;
    unsigned char *mTextureBuffer;
};
} // namespace Nova
