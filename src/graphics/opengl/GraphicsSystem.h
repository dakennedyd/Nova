
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
//#include <GL/glew.h> // GL extension handler library
#include "ISingleton.h"
#include "ISubSystem.h"
#include "graphics/IGraphicsSystem.h"
#include "graphics/IRendererBackend.h"
#include "graphics/IRendererFrontend.h"
#include "graphics/opengl/Light.h"
#include "graphics/opengl/RenderPacket.h"
#include "math/Matrix.h"
#include "math/Vector.h"
#include <cassert>
#include <memory>

namespace Nova
{
struct CameraInfo
{
    Mat4 *viewMatrix;
    Mat4 *projectionMatrix;
    Vec3 *position;
    Vec3 *forwardVector;
};

class RenderPacket;
class RendererBackend;
class GraphicsSystem final : public ISingleton<GraphicsSystem>,
                             public IGraphicsSystem,
                             public ISubSystem
{
  public:
    GraphicsSystem() = default;
    ~GraphicsSystem() = default;
    void startUp() override;
    void shutDown() override;
    /*void setCurrentViewMatrix(Mat4& view) { mCurrentCamera.viewMatrix = &view; };
    void setCurrentProjMatrix(Mat4& projection) { mCurrentCamera.projectionMatrix = &projection; };
    void setCurrentCameraPosition(Vec3& position) { mCurrentCamera.position = &position; };
    void setCurrentCameraForward
    Mat4& getCurrentViewMatrix() const { return *mCurrentCamera.viewMatrix; };
    Mat4& getCurrentProjMatrix() const { return *mCurrentCamera.projectionMatrix; };*/
    void setCurrentCamera(CameraInfo camera) { mCurrentCamera = camera; };
    CameraInfo &getCurrentCamera() { return mCurrentCamera; };
    void checkExtensionsSupport();
    GLfloat getMaxAnisotropy();
    void setWireframeMode(bool state);

    // void setCurrentRenderer(IRendererBackend *renderer);

    /*adds a render packet to the renderer's list so the renderer can see it*/
    inline void addPacket(RenderPacket packet)
    {
        // uint64_t returnValue = packet.getID();
        // mRenderPackets.emplace(std::make_pair(returnValue, std::move(packet)));
        // return returnValue;
        mRenderPackets.push_back(std::move(packet));
    };

    // void removePacket(std::uint64_t id) { mRenderPackets.erase(id); };

    inline void addLight(Light light)
    {
        // uint64_t returnValue = light.getID();
        // mLights.emplace(std::make_pair(returnValue, std::move(light)));
        // return returnValue;
        mLights.push_back(std::move(light));
    }

    // void removeLight(std::uint64_t id) { mLights.erase(id); };
    auto &getRenderPackets() { return mRenderPackets; };
    auto &getLights() { return mLights; }
    IRendererBackend &getRendererBackend()
    {
        assert(mRendererBackend);
        return *mRendererBackend;
    };
    IRendererFrontend &getRendererFrontend()
    {
        assert(mRendererFrontend);
        return *mRendererFrontend;
    };
    void setRendererBackend(std::shared_ptr<IRendererBackend> backend)
    {
        mRendererBackend = backend;
    };
    void setRendererFrontend(std::shared_ptr<IRendererFrontend> frontend)
    {
        mRendererFrontend = frontend;
    };

  private:
    // Camera* mCurrentCamera;	/*this should be replaced with a layer
    // stack each one with its own camera*/
    CameraInfo mCurrentCamera;

    // std::unordered_map<uint64_t, RenderPacket> mRenderPackets;
    // std::unordered_map<uint64_t, Light> mLights;
    std::vector<RenderPacket> mRenderPackets;
    std::vector<Light> mLights;

    std::shared_ptr<IRendererBackend> mRendererBackend;
    std::shared_ptr<IRendererFrontend> mRendererFrontend;
    GLfloat mMaxAnisotropyLevel = 1.0f; // max anisotropic filtering level supported
};
} // namespace Nova
