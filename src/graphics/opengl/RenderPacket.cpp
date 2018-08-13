
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

#include "RenderPacket.h"
#include "ECS/Entity.h"
#include "PreInit.h"
#include "graphics/opengl/GPUProgramParameter.h"
#include "graphics/opengl/GraphicsSystem.h"
#include "graphics/opengl/Material.h"
#include "graphics/opengl/Mesh.h"
#ifdef NOVA_OPENGL
#    include "graphics/opengl/RendererInit.h"
#endif
#include "ECS/DefaultComponents.h"
#include <memory>
#include <vector>

namespace Nova
{
void RenderPacket::draw() const
{
    glDrawElements(GL_TRIANGLES, mMesh->getNumIndices(), GL_UNSIGNED_INT, 0);
}

void inline updateLights(GLuint id)
{
    auto &lightsList = GraphicsSystem::getInstance().getLights();
    std::size_t i = 0;
    for (auto &light : lightsList)
    {
        if (i >= MAX_LIGHTS) break;
        glUniform3fv(
            glGetUniformLocation(id, ("light[" + std::to_string(i) + "].position").c_str()), 1,
            light.getPosition()->getDataPtr());
        glUniform3fv(glGetUniformLocation(id, ("light[" + std::to_string(i) + "].color").c_str()),
                     1, light.getColor()->getDataPtr());
        glUniform1i(glGetUniformLocation(id, ("light[" + std::to_string(i) + "].type").c_str()),
                    light.getTypeCode());
        i++;
    }
}

void inline updateCamera(GLuint id)
{
    auto &camera = GraphicsSystem::getInstance().getCurrentCamera();
    glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_FALSE, camera.view->getDataPtr());
    glUniformMatrix4fv(glGetUniformLocation(id, "proj"), 1, GL_FALSE,
                       camera.projection->getDataPtr());
    glUniform3fv(glGetUniformLocation(id, "cameraPos"), 1, camera.position->getDataPtr());
}

RenderPacket::RenderPacket(const std::shared_ptr<Mesh> &mesh,
                           const std::shared_ptr<Material> &material, const Transform &transform)
    : mMesh(mesh), mMaterial(material)
{
    // temporary, probably need a custom memory allocator
    GLuint id = mMaterial->getGPUProgram()->getProgramID();
    // auto &gs = GraphicsSystem::getInstance();

    addParameter(new GPUProgramParameterMat4(glGetUniformLocation(id, "model"),
                                             transform.finalTransform.getDataPtr()));
    addParameter(new GPUProgramParameterMat4(glGetUniformLocation(id, "normalMat"),
                                             transform.normalMatrix.getDataPtr()));
    // addParameter(new GPUProgramParameterMat4(glGetUniformLocation(id, "view"),
    //                                          gs.getCurrentCamera().view->getDataPtr()));
    // addParameter(new GPUProgramParameterMat4(glGetUniformLocation(id, "proj"),
    //                                          gs.getCurrentCamera().projection->getDataPtr()));
}

RenderPacket::RenderPacket(const std::shared_ptr<Mesh> &mesh,
                           const std::shared_ptr<Material> &material)
    : mMesh(std::move(mesh)), mMaterial(std::move(material))
{
}

/*todo: this function should be replaced by the ECSystem*/
void RenderPacket::updateAllUniforms() const
{
    updateCamera(mMaterial->getGPUProgram()->getProgramID());
    updateLights(mMaterial->getGPUProgram()->getProgramID());
    for (IGPUProgramParameter *parameter : mParameters)
    {
        parameter->update();
    }
}

RenderPacket::~RenderPacket()
{
    for (IGPUProgramParameter *parameter : mParameters)
    {
        delete parameter;
    }
}

RenderPacket::RenderPacket(RenderPacket &&other)
    : mMesh(std::move(other.mMesh)), mMaterial(std::move(other.mMaterial)),
      /*mTransform(other.mTransform),*/ mParameters(
          std::move(other.mParameters)) /*mID(other.mID),*/
{
    other.mParameters = std::vector<IGPUProgramParameter *>{};
}

RenderPacket &RenderPacket::operator=(RenderPacket &&other)
{
    // return std::move(RenderPacket(std::move(otherGP.mMesh), std::move(otherGP.mMaterial),
    // std::move(otherGP.mTransform))); return std::move(returnValue);
    if (&other != this)
    {
        mMesh = std::move(other.mMesh);
        mMaterial = std::move(other.mMaterial);
        mParameters = std::move(other.mParameters);
        // otherGP.mParameters = std::vector<IGPUProgramParameter*>{};
        // mID = other.mID;
        // mTransform = std::move(other.mTransform);
    }
    return *this;
}

void RenderPacket::bind() const
{
    mMesh->bind();
    mMaterial->bind();
}
void RenderPacket::unBind() const
{
    mMesh->unBind();
    mMaterial->unBind();
}
} // namespace Nova