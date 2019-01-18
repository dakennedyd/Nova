
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

#include "FrameBuffer.h"
#include "Error.h"
#include "graphics/opengl/Texture.h"
#include "logger/Logger.h"
#include <glad/glad.h>
#include <memory>

namespace Nova
{
FrameBuffer::FrameBuffer() { glGenFramebuffers(1, &mFrameBufferID); }
void FrameBuffer::bind() const { glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID); }
void FrameBuffer::unBind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
void FrameBuffer::attachTexture(const std::shared_ptr<Texture> texture)
{
    TextureType texType = texture->getTextureType();

    // check limits
    int maxFBWidth, maxFBHeight;
    glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &maxFBWidth);
    glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &maxFBHeight);
    if (texture->getWidth() > maxFBWidth)
    {
        error("framebuffer width limit exceeded");
    };
    if (texture->getHeight() > maxFBHeight)
    {
        error("framebuffer height limit exceeded");
    };
    if (texture->getWidth() < 1)
    {
        error("Invalid framebuffer width");
    };
    if (texture->getHeight() < 1)
    {
        error("Invalid framebuffer height");
    };

    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
    if (texType == TextureType::DEPTHBUFFER)
    {
        // if (mDepthBuffer != nullptr) LOG_WARNING("overwriting framebuffer depth attachment");
        mDepthBuffer = texture;
        // attach the RBO to the framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                                  texture->getTextureID());
    }
    else if (texType == TextureType::DEPTHSTENCIL)
    {
        mDepthBuffer = texture;
        mStencilBuffer = texture;
        // attach the RBO to the framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                  texture->getTextureID());
    }
    else if (texType == TextureType::COLOR || texType == TextureType::COLOR_ALPHA ||
             texType == TextureType::COLOR_ALPHA || texType == TextureType::GBUFFER_NORMAL ||
             texType == TextureType::GBUFFER_POSITION || texType == TextureType::NORMAL_MAP ||
             texType == TextureType::COLOR_HDR || texType == TextureType::MONOCHROME)
    {
        int maxColorAttachments = getMaxColorAttachments();
        // glGetIntegerv(getMaxColorAttachments(), &maxColorAttachments);
        if (static_cast<int>(mColorTextures.size()) + 1 > maxColorAttachments)
            LOG_ERROR("too many color buffers on framebuffer");

        mColorTextures.push_back(texture);

        // attach color texture(s) to the framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + static_cast<int>(mColorTextures.size()) - 1,
                               GL_TEXTURE_2D, texture->mTextureID, 0);
    }
    else
    {
        LOG_ERROR("Unrecognized texture type for framebuffer attachment")
    };
    ////check to see if the framebuffer is ready to use
    // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //{
    //	//CHECK_OPEGL_ERROR();
    //	error("Cannot create framebuffer");
    //}
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffer::bindAllTextutes()
{
    int index = 0;
    for (auto &texture : mColorTextures)
    {
        glActiveTexture(GL_TEXTURE0 + index);
        texture->bind();
        // glUniform1i(glGetUniformLocation(mProgram->getProgramID(), mSamplerNames[index].c_str()),
        // index);

        index++;
    }
}
void FrameBuffer::unBindAllTextures()
{
    for (auto &texture : mColorTextures)
    {
        texture->unBind();
    }
    glActiveTexture(GL_TEXTURE0);
}
// FrameBuffer::FrameBuffer(const int width, const int height, const bool hasDepth,
//	const bool hasStencil, const int colorBuffers)
//	:mHasDepth(hasDepth), mHasStencil(hasStencil)
//	//mTexture(width, height, nullptr, false, false)
//{
//	int maxFBWidth, maxFBHeight;
//	glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &maxFBWidth);
//	glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &maxFBHeight);
//	if (width > maxFBWidth) { error("framebuffer width limit exceeded"); };
//	if (height > maxFBHeight) { error("framebuffer height limit exceeded"); };
//	if (width < 1) { error("Invalid framebuffer width"); };
//	if (height < 1) { error("Invalid framebuffer height"); };
//	//mWidth = width;
//	//mHeight = height;
//	//mTextures.emplace_back(Texture(width, height, nullptr, false, false));
//	//mTexture = std::make_shared<Texture>(width, height, nullptr, false, false);

//	glGenFramebuffers(1, &mFrameBufferID);
//	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);

//	int maxColorAttachments;
//	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
//	if (colorBuffers > maxColorAttachments) LOG_ERROR("too many color buffers on framebuffer");
//	for (int i = 0; i < colorBuffers; i++)
//	{
//		mColorTextures.emplace_back(std::make_shared<Texture>(width, height, nullptr,
// TextureType::COLOR, 			Filtering::NEAREST));

//		//attach color texture(s) to the framebuffer
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
//			mColorTextures[i]->mTextureID, 0);
//	}
//	if (hasDepth && !hasStencil && colorBuffers > 0)
//	{
//		//create a render buffer object for the depth buffer
//		glGenRenderbuffers(1, &mDepthBufferID);
//		glBindRenderbuffer(GL_RENDERBUFFER, mDepthBufferID);
//		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
//		glBindRenderbuffer(GL_RENDERBUFFER, 0);

//		//attach the RBO to the framebuffer
//		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
//			GL_RENDERBUFFER, mDepthBufferID);
//	}
//	else if (hasDepth && !hasStencil && colorBuffers == 0) //shadow map?
//	{
//		mColorTextures.emplace_back(std::make_shared<Texture>(Texture::makeShadowMap(width,
// height)));
//		//attach shadowmap to the framebuffer
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
//			mColorTextures[0]->mTextureID, 0);
//
//		// Instruct openGL that we won't bind a color texture with the currently bound FBO
//		glDrawBuffer(GL_NONE);
//		glReadBuffer(GL_NONE);

//		//for some reason opengl doesn't let me create a fb without a color attachment
//		// even with glDrawBuffer(GL_NONE);
//		mColorTextures.emplace_back(std::make_shared<Texture>(1, 1, nullptr,
// TextureType::COLOR, Filtering::NEAREST)); 		glFramebufferTexture2D(GL_FRAMEBUFFER,
// GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTextures[1]->mTextureID, 0);
//	}
//	else if (hasDepth && hasStencil)
//	{
//		//create a render buffer object for the depth and stencil buffers
//		glGenRenderbuffers(1, &mDepthStencilRenderBufferID);
//		glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilRenderBufferID);
//		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
//		glBindRenderbuffer(GL_RENDERBUFFER, 0);

//		//attach the RBO to the framebuffer
//		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
//			GL_RENDERBUFFER, mDepthStencilRenderBufferID);
//	}

//	//check to see if the framebuffer is ready to use
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//	{
//		//CHECK_OPEGL_ERROR();
//		error("Cannot create framebuffer");
//	}
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}

FrameBuffer::~FrameBuffer()
{
    // framebuffer mantains a vector of texture smart pointers
    // that should be destroyed automatically when they
    // are not needed anymore
    // if (mHasDepth && mHasStencil) glDeleteRenderbuffers(1, &mDepthStencilRenderBufferID);

    glDeleteFramebuffers(1, &mFrameBufferID);
}
FrameBuffer::FrameBuffer(FrameBuffer &&other)
    : mFrameBufferID(other.mFrameBufferID), mDepthBuffer(std::move(other.mDepthBuffer)),
      mStencilBuffer(std::move(other.mStencilBuffer)),
      // mWidth(std::move(other.mWidth)), mHeight(std::move(other.mHeight)),
      // mHasDepth(std::move(other.mHasDepth)), mHasStencil(std::move(other.mHasStencil)),
      mColorTextures(std::move(other.mColorTextures))
{
    other.mFrameBufferID = 0;
}
FrameBuffer &FrameBuffer::operator=(FrameBuffer &&other)
{
    if (&other != this)
    {
        mFrameBufferID = std::move(other.mFrameBufferID);
        mDepthBuffer = std::move(other.mDepthBuffer);
        mStencilBuffer = std::move(other.mStencilBuffer);
        // mWidth = std::move(other.mWidth);
        // mHeight = std::move(other.mHeight);
        // mHasDepth = std::move(other.mHasDepth);
        // mHasStencil = std::move(other.mHasStencil);
        mColorTextures = std::move(other.mColorTextures);
    }
    return *this;
}
const std::shared_ptr<Texture> FrameBuffer::getColorTexture(const int textureNumber)
{
    /*return std::shared_ptr<Texture>(&(mTextures[0]), [](Texture *) {});*/
    // return std::shared_ptr<Texture>(&mTexture, [](Texture *) {});
    // LOG_DEBUG("fb color tex array size:" << mColorTextures.size());
    if (textureNumber > static_cast<int>(mColorTextures.size()) - 1)
        LOG_ERROR("tried to get invalid color texture on framebuffer");
    return mColorTextures[textureNumber];
}
const std::shared_ptr<Texture> FrameBuffer::getDepthBuffer()
{
    return mDepthBuffer;
}
FrameBuffer FrameBuffer::makeDefaultFrameBuffer(const int width, const int height)
{
    FrameBuffer fb;
    fb.attachTexture(
        std::make_shared<Texture>(width, height, nullptr, TextureType::COLOR, Filtering::NEAREST));
    fb.attachTexture(std::make_shared<Texture>(width, height, nullptr, TextureType::DEPTHBUFFER,
                                               Filtering::NEAREST));

    if (!fb.isComplete())
        LOG_ERROR("can't create color/depth framebuffer"); // check for framebuffer completeness
    return fb;
}
FrameBuffer FrameBuffer::makeGBuffer(const int width, const int height)
{
    FrameBuffer fb;
    fb.attachTexture(std::make_shared<Texture>(width, height, nullptr,
                                               TextureType::GBUFFER_POSITION, Filtering::NEAREST,
                                               MipMapping::NO_MIPMAP));
    fb.attachTexture(std::make_shared<Texture>(width, height, nullptr, TextureType::GBUFFER_NORMAL,
                                               Filtering::NEAREST, MipMapping::NO_MIPMAP));
    fb.attachTexture(std::make_shared<Texture>(width, height, nullptr, TextureType::COLOR_HDR,
                                               Filtering::NEAREST, MipMapping::NO_MIPMAP));
    fb.attachTexture(std::make_shared<Texture>(width, height, nullptr, TextureType::NORMAL_MAP,
                                               Filtering::NEAREST, MipMapping::NO_MIPMAP));
    fb.attachTexture(std::make_shared<Texture>(width, height, nullptr, TextureType::DEPTHBUFFER,
                                               Filtering::NEAREST, MipMapping::NO_MIPMAP));

    fb.bind();
    fb.mAttachments[0] = GL_COLOR_ATTACHMENT0;
    fb.mAttachments[1] = GL_COLOR_ATTACHMENT1;
    fb.mAttachments[2] = GL_COLOR_ATTACHMENT2;
    fb.mAttachments[3] = GL_COLOR_ATTACHMENT3;

    glDrawBuffers(4, fb.mAttachments);
    fb.unBind();
    if (!fb.isComplete()) LOG_ERROR("can't create GBuffer"); // check for framebuffer completeness
    return fb;
}
FrameBuffer FrameBuffer::makePostProcessFrameBuffer(const int width, const int height)
{
    FrameBuffer fb;
    fb.attachTexture(std::make_shared<Texture>(width, height, nullptr, TextureType::COLOR_HDR,
                                               Filtering::LINEAR, MipMapping::GENERATE_MIPMAP));
    fb.getColorTexture(0)->bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    fb.getColorTexture(0)->unBind();

    if (!fb.isComplete())
        LOG_ERROR("can't create post process framebuffer"); // check for framebuffer completeness
    return fb;
}
const bool FrameBuffer::isComplete()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
    // check to see if the framebuffer is ready to use
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return true;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return false;
}
} // namespace Nova