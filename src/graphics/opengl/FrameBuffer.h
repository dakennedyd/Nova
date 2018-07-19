
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
#include "graphics/IFrameBuffer.h"
//#include "graphics/opengl/Texture.h"
#include "RendererInit.h"
#include <memory>
#include <vector>

namespace Nova
{
class Texture;
class FrameBuffer : public IFrameBuffer
{
  public:
    FrameBuffer();
    // FrameBuffer(Texture& colorBuffer);

    /*FrameBuffer(const int width, const int height, const bool hasDepth,
            const bool hasStencil, const int colorBuffers = 1);*/
    ~FrameBuffer();
    FrameBuffer(const FrameBuffer &) = delete;            // copy ctor
    FrameBuffer(FrameBuffer &&other);                     // move ctor
    FrameBuffer &operator=(FrameBuffer const &) = delete; // copy assignment op
    FrameBuffer &operator=(FrameBuffer &&other);          // move assignment op

    void bind() const override;
    void unBind() const override;

    const std::shared_ptr<Texture> getColorTexture(const int textureNumber);

    static FrameBuffer makeShadowMapFrameBuffer(const int width = DEFAULT_SHADOW_MAP_WIDTH,
                                                const int height = DEFAULT_SHADOW_MAP_HEIGHT)
    {
        // return FrameBuffer(width, height, true, false, 0);
        return FrameBuffer();
    }

    static FrameBuffer makeDefaultFrameBuffer(const int width, const int height);
    static FrameBuffer makeGBuffer(const int width, const int height);
    static FrameBuffer makePostProcessFrameBuffer(const int width, const int height);
    // auto getWidth() { return mWidth; }
    // auto getHeight() { return mHeight; }
    const bool isComplete();

    void bindAllTextutes(); // binds all color textures attached to this framebuffer
    void unBindAllTextures();

  private:
    // attaches textures to the framebuffer, after attaching all the textures
    // you have to check for FB completeness
    void attachTexture(const std::shared_ptr<Texture> texture);
    GLuint mFrameBufferID;
    // GLuint mDepthBufferID;
    // GLuint mDepthStencilRenderBufferID;
    std::shared_ptr<Texture> mDepthBuffer = nullptr;
    std::shared_ptr<Texture> mStencilBuffer = nullptr;
    // std::vector<Texture> mTextures;
    // int mWidth;
    // int mHeight;
    // bool mHasDepth = false;
    // bool mHasStencil = false;
    std::vector<std::shared_ptr<Texture>> mColorTextures;
    // Texture& mColorTexture;
    GLenum mAttachments[8]; // GL3.3 provides at least 8 color attachments
};

} // namespace Nova