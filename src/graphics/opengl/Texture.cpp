
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

#include "Texture.h"
#include "Error.h"
#include "PreInit.h"
#include "graphics/opengl/GraphicsSystem.h"
#include "logger/Logger.h"
#ifdef NOVA_WINDOWS_PLATFORM
#    include "windows/FileSystem.h"
#else
#    include "linux/FileSystem.h"
#endif
//#include <glad/glad.h>
#include <utility>
//#include <GLFW/glfw3.h>

namespace Nova
{
Texture::Texture(const int width, const int height, const void *textureData,
                 /*const bool generateMipmap, const bool hasAlphaChannel,*/
                 const enum TextureType &textureType, const enum Filtering &filtering,
                 const enum MipMapping &mipmap)
    //:mHasMipmaps(generateMipmap)
    : mType(textureType), mHasMipmaps(mipmap)
{
    // mHasMipmaps = true;
    if (width < 1)
    {
        error("Invalid texture width");
    };
    mWidth = width;
    if (height < 1)
    {
        error("Invalid texture height");
    };
    mHeight = height;
    int maxTextureSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    if (width > maxTextureSize || height > maxTextureSize)
    {
        error("Texture dimension too big");
    };

    if (textureType == TextureType::DEPTHBUFFER || textureType == TextureType::DEPTHSTENCIL)
    {
        // mBorderColor[0] = 0.0f; mBorderColor[1] = 0.0f; mBorderColor[2] = 0.0f; mBorderColor[3] =
        // 0.0f;
        // Set the texture wrapping option
        // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, mBorderColor);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // filtering
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // signals GPU to upload texture data
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
        // width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        // create a render buffer object for the depth buffer

        glGenRenderbuffers(1, &mTextureID);
        glBindRenderbuffer(GL_RENDERBUFFER, mTextureID);
        if (textureType == TextureType::DEPTHBUFFER)
        {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        }
        else if (textureType == TextureType::DEPTHSTENCIL)
        {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        }
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
    else
    {

        glGenTextures(1, &mTextureID);
        glBindTexture(GL_TEXTURE_2D, mTextureID);

        GLfloat maxAniso = GraphicsSystem::getInstance().getMaxAnisotropy(); // set anisoptropy
                                                                             // level
        if (maxAniso >= 4.0f) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
    }

    if (textureType == TextureType::GBUFFER_NORMAL ||
        textureType == TextureType::GBUFFER_POSITION || textureType == TextureType::NORMAL_MAP)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        if (textureType == TextureType::GBUFFER_NORMAL)
        {
            // if i choose a bit depth lower than 32F i get glitches why?? GL_RGB10_A2
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
        }
        if (textureType == TextureType::GBUFFER_POSITION)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
        }
        if (textureType == TextureType::NORMAL_MAP)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
        }
    }
    if (textureType == TextureType::COLOR || textureType == TextureType::COLOR_ALPHA ||
        textureType == TextureType::MONOCHROME || textureType == TextureType::DUAL_COLOR ||
        textureType == TextureType::COLOR_HDR)
    {
        // Set the texture wrapping option
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        // Set the texture filtering options
        if (filtering == Filtering::LINEAR)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            if (mHasMipmaps)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            else
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
        else if (filtering == Filtering::NEAREST)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            if (mHasMipmaps)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            else
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
        if (textureType == TextureType::COLOR_ALPHA)
        {
            // signals GPU to upload texture data
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         textureData);
        }
        if (textureType == TextureType::COLOR)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         textureData);
        }
        if (textureType == TextureType::COLOR_HDR)
        {
            // mHasMipmaps = false; //this is ugly
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT,
                         textureData);
        }
        if (textureType == TextureType::MONOCHROME)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE,
                         textureData);
        }
        if (textureType == TextureType::DUAL_COLOR)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE,
                         textureData);
        }
        if (mHasMipmaps)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    // CHECK_OPEGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureType selectTextureType(const TextureInfo &textureInfo)
{
    if (textureInfo.bpp == 4)
        return TextureType::COLOR_ALPHA;
    else if (textureInfo.bpp == 3)
    {
        if (textureInfo.isHDR)
            return TextureType::COLOR_HDR;
        else
            return TextureType::COLOR;
    }
    else if (textureInfo.bpp == 2)
        return TextureType::DUAL_COLOR;
    else if (textureInfo.bpp == 1)
        return TextureType::MONOCHROME;
    else
    {
        LOG_WARNING("texture depth:" << textureInfo.bpp);
        error("unrecognized texture buffer");
        return TextureType::COLOR;
    }
}

Texture::Texture(const TextureInfo &textureInfo)
    : Texture(textureInfo.width, textureInfo.height,
              static_cast<const GLubyte *>(textureInfo.textureBuffer[0]), // this is not smart...
              selectTextureType(textureInfo))
//(textureInfo.bpp == 4) ? TextureType::COLOR_ALPHA : (textureInfo.bpp == 3) ? TextureType::COLOR :
// unrecognizedTextureBuffer()) TextureType::COLOR)
{
    // bpp == 4 -> GL_RGBA
    // bpp == 3 -> GL_RGB
    // mTextureBuffer = textureInfo.textureBuffer;
    FileSystem::getInstance().freeTexture(
        textureInfo
            .textureBuffer[0]); // TODO: texture object should hold a ptr to image data in ram
}

Texture::Texture(const std::string &fileAndPath)
    : Texture(FileSystem::getInstance().loadTexture(fileAndPath, true))
{
    // auto texInfo = FileSystem::getInstance().loadTexture(fileAndPath, true);
}

Texture::~Texture()
{
    if (mType == TextureType::DEPTHBUFFER)
    {
        glDeleteRenderbuffers(1, &mTextureID);
    }
    else
    {
        glDeleteTextures(1, &mTextureID);
    }
    // if (mTextureBuffer != nullptr) FileSystem::getInstance().freeTexture(mTextureBuffer);
}
Texture::Texture(Texture &&other)
    : mTextureID(std::move(other.mTextureID)), mWidth(std::move(other.mWidth)),
      mHeight(std::move(other.mHeight)), mType(std::move(other.mType)),
      mHasMipmaps(std::move(other.mHasMipmaps)), mTextureBuffer(std::move(other.mTextureBuffer))
{
    mBorderColor[0] = std::move(other.mBorderColor[0]);
    mBorderColor[1] = std::move(other.mBorderColor[1]);
    mBorderColor[2] = std::move(other.mBorderColor[2]);
    mBorderColor[3] = std::move(other.mBorderColor[3]);
    mTextureID = 0;
    mTextureBuffer = nullptr;
}
Texture &Texture::operator=(Texture &&other)
{
    if (&other != this)
    {
        mTextureID = std::move(other.mTextureID);
        mWidth = std::move(other.mWidth);
        mHeight = std::move(other.mHeight);
        mHasMipmaps = std::move(other.mHasMipmaps);
        mType = std::move(other.mType);
        mTextureBuffer = std::move(other.mTextureBuffer);
        // mTextureBuffer = other.mTextureBuffer;
        mBorderColor[0] = std::move(other.mBorderColor[0]);
        mBorderColor[1] = std::move(other.mBorderColor[1]);
        mBorderColor[2] = std::move(other.mBorderColor[2]);
        mBorderColor[3] = std::move(other.mBorderColor[3]);
    }
    return *this;
}
void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, mTextureID); }
void Texture::unBind() const { glBindTexture(GL_TEXTURE_2D, 0); }
void Texture::setFiltering(const Filtering &option)
{
    bind();
    switch (option)
    {
    case LINEAR:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if (mHasMipmaps)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        break;
    case NEAREST:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        if (mHasMipmaps)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        break;
    }
    unBind();
}
} // namespace Nova