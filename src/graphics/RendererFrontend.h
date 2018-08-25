
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
#include "IRendererFrontend.h"
#include "PreInit.h"
#ifdef NOVA_OPENGL
#    include "graphics/opengl/GraphicsSystem.h"
#endif
#include "Application.h"
#include "ECS/DefaultSystems.h"

namespace Nova
{

class RendererFrontend : public IRendererFrontend
{
  public:
    RendererFrontend();
    ~RendererFrontend() = default;
    // inline void createRenderPackets() override
    // {
    //     auto lightSystem = Application::getInstance().getWorld().GetSystem<LightSystem>();
    //     auto &lightEntities = lightSystem->getEntities();
    //     auto &graphicsSystem = GraphicsSystem::getInstance();
    //     for (auto &numberEntityPair : lightEntities) // for each entity that produces light
    //     {
    //         auto &lc = numberEntityPair.second->GetComponent<LightComponent>();
    //         // int type = 1;
    //         // if (lc.type == LightType::POINT_LIGHT) type = 1;
    //         // else if(lc.type == LightType::DIRECTIONAL_LIGHT) type = 0;
    //         graphicsSystem.addLight(Light{
    //             lc.type,
    //             &(numberEntityPair.second->getNonConstTransformStruct().finalTranslation),
    //             &(lc.color)});
    //     }

    //     const auto &visualSystem =
    //     Application::getInstance().getWorld().GetSystem<VisualSystem>(); auto &visualEntities =
    //     visualSystem->getEntities(); for (auto &idEntityPair : visualEntities) // for each
    //     registered renderable entity
    //     {
    //         auto &vc = idEntityPair.second->GetComponent<VisualComponent>();
    //         graphicsSystem.addPacket(
    //             RenderPacket{vc.mesh, vc.material, idEntityPair.second->getTransformStruct()});
    //     }
    // }
};
} // namespace Nova
