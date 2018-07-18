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