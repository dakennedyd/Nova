
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
#include "GraphicsSystem.h"
#include "Mesh.h"
#include "Physics.h"
#include <memory>

namespace Nova
{
// class Mesh;
class TextureCube;
struct PBRSkybox : public ResourceBase
{
    PBRSkybox(const std::shared_ptr<TextureCube> &skyboxTexture,
              const std::shared_ptr<TextureCube> &irradiance,
              const std::shared_ptr<TextureCube> &radiance, const std::shared_ptr<Texture> &LUT)
        : texture(skyboxTexture), radiance(radiance), irradiance(irradiance), BRDFLUT(LUT){};

    std::shared_ptr<TextureCube> texture;
    std::shared_ptr<TextureCube> radiance;
    std::shared_ptr<TextureCube> irradiance;
    std::shared_ptr<Texture> BRDFLUT;
};
// struct Skybox
// {
//     std::shared_ptr<TextureCube> skyboxTexture;
//     std::shared_ptr<IBL_Data> iblData;
// };

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
    void setSkyBox(const std::shared_ptr<PBRSkybox> &skyBox) override;

    std::unordered_map<std::string, long> &getProfileTimes() override { return mProfileTimes; };

    void drawLine(const Vec3 &from, const Vec3 &to, const Vec3 &color = Vec3(1.0f)) override;
    void addLight() override;    
    void removeLight() override;
    
  private:
    void inline updateLights();
    void PhysicsDebugDraw() { Physics::getInstance().mDynamicsWorld->debugDrawWorld(); };
    // bool mIsInitialized = false;
    int mWidth = Window::getInstance().getWidth();
    int mHeight = Window::getInstance().getHeight();
    std::shared_ptr<Mesh> mScreenQuad = std::make_shared<Mesh>(Mesh::makeQuad(2.0f, 2.0f));
    std::shared_ptr<PBRSkybox> mCurrentSkybox = nullptr;
    FrameBuffer mGBuffer;
    FrameBuffer mLightPassFrameBuffer;
    FrameBuffer mHBloomFrameBuffer[5];
    FrameBuffer mVBloomFrameBuffer[5];

    RenderPacket mLightPassRenderPacket;
    RenderPacket mHBloomPacket[5];
    RenderPacket mVBloomPacket[5];
    RenderPacket mCurrentSkyBoxPacket;
    RenderPacket mFinalPacket;

    // long mGPassTime, mLightPassTime, mPostprocessTime;
    std::unordered_map<std::string, long> mProfileTimes;

    // std::vector<std::pair<FrameBuffer, GPUProgram>> mFrameBuffers;
};
} // namespace Nova
