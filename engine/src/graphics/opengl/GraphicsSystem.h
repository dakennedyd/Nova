
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
#include "ECS/DefaultComponents.h"
#include "ISingleton.h"
#include "ISubSystem.h"
#include "audio/Audio.h"
#include "graphics/IGraphicsSystem.h"
#include "graphics/IRendererBackend.h"
#include "graphics/IRendererFrontend.h"
#include "graphics/opengl/Light.h"
#include "graphics/opengl/RenderPacket.h"
#include "math/Matrix.h"
#include "math/Vector.h"
#include <cassert>
#include <map>
#include <memory>
//#include <unordered_map>

namespace Nova
{
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
    void checkExtensionsSupport();
    GLfloat getMaxAnisotropy();
    void setWireframeMode(bool state);

    // void setCurrentRenderer(IRendererBackend *renderer);

    /*adds a render packet to the renderer's list so the renderer can see it*/
    inline void addPacket(RenderPacket packet)
    {
        mRenderPackets.emplace(std::make_pair(packet.getID(), std::move(packet)));
        // mRenderPackets.push_back(std::move(packet));
    };
    void removePacket(std::uint64_t id) { mRenderPackets.erase(id); };
    inline void addLight(Light light)
    {
        //currentNumLights++;
        mLights.emplace(std::make_pair(light.getID(), std::move(light)));
        getRendererBackend().addLight();
        // mLights.push_back(std::make_pair(light.getID(), std::move(light)));
    }
    void removeLight(std::uint64_t id)
    {
        //assert(mLights.erase(id));
        mLights.erase(id);
        getRendererBackend().removeLight();
        //currentNumLights--;
    };
    auto &getRenderPackets() { return mRenderPackets; };
    auto &getLights() { return mLights; };
    int getNumLights() { return mLights.size(); };
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
    void setCurrentCamera(Entity *camera) override
    {
        // mCurrentCamera = camera;
        mCamera.projection = &(camera->getComponent<CameraComponent>().projection);
        mCamera.view = &(camera->getComponent<CameraComponent>().view);
        mCamera.position = &(camera->getNonConstTransformStruct().finalTranslation);
        Audio::getInstance().assignListener(*camera);
    };
    Camera &getCurrentCamera() override { return mCamera; };
    std::size_t currentNumLights = 0;

  private:
    std::map<uint64_t, RenderPacket> mRenderPackets;
    std::map<uint64_t, Light> mLights;
    // std::vector<std::pair<uint64_t, Light>> mLights;
    // std::vector<RenderPacket> mRenderPackets;
    // std::vector<Light> mLights;

    std::shared_ptr<IRendererBackend> mRendererBackend;
    std::shared_ptr<IRendererFrontend> mRendererFrontend;
    GLfloat mMaxAnisotropyLevel = 1.0f; // max anisotropic filtering level supported
    // Entity *mCurrentCamera;
    Camera mCamera;
};
} // namespace Nova
