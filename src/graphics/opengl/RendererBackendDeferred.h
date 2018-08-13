
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
#include "RenderPacket.h"
#include "graphics/IRendererBackend.h"
#include "graphics/opengl/FrameBuffer.h"
#include "resource_manager/ResourceBase.h"
#ifdef NOVA_WINDOWS_PLATFORM
#    include "windows/Window.h"
#else
#    include "linux/Window.h"
#endif
//#include "ECS/DefaultComponents.h"
#include "ECS/Entity.h"
#include "Mesh.h"
#include <memory>

namespace Nova
{
// class Mesh;
class TextureCube;
struct IBL_Data : public ResourceBase
{
    IBL_Data(const std::shared_ptr<TextureCube> &irradiance,
             const std::shared_ptr<TextureCube> radiance)
        : radiance(radiance), irradiance(irradiance){};

    std::shared_ptr<TextureCube> radiance;
    std::shared_ptr<TextureCube> irradiance;
    // std::vector<std::shared_ptr<TextureCube>> radiance;
};
struct Skybox
{
    std::shared_ptr<TextureCube> skyboxTexture;
    std::shared_ptr<IBL_Data> iblData;
};

// represents one link in the chain of events that
// the renderer goes trhough to render a frame
struct ChainLink
{
};

class RendererBackendDeferred : public IRendererBackend
{
  public:
    RendererBackendDeferred();
    ~RendererBackendDeferred() = default;

    void init() override;
    void render() override;
    void setSkyBox(const Skybox &skyBox) override;
    void setIBLData(std::shared_ptr<IBL_Data> data);

  private:
    // bool mIsInitialized = false;
    int mWidth = Window::getInstance().getWidth();
    int mHeight = Window::getInstance().getHeight();
    std::shared_ptr<Mesh> mScreenQuad = std::make_shared<Mesh>(Mesh::makeQuad(2.0f, 2.0f));
    IBL_Data mIBL;
    FrameBuffer mGBuffer;
    FrameBuffer mLightPassFrameBuffer;
    FrameBuffer mHBloomFrameBuffer;
    FrameBuffer mVBloomFrameBuffer;

    RenderPacket mLightPassRenderPacket;
    RenderPacket mFinalPacket;
    RenderPacket mHBloomPacket;
    RenderPacket mVBloomPacket;
    RenderPacket mCurrentSkyBox;

    // std::vector<std::pair<FrameBuffer, GPUProgram>> mFrameBuffers;
};
} // namespace Nova
