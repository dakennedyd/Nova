
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

#include "RendererBackendDeferred.h"
#include "Timer.h"
#include "graphics/DebugUI.h"
#include "graphics/ITexture.h"
#include "graphics/opengl/FrameBuffer.h"
#include "graphics/opengl/GPUProgram.h"
#include "graphics/opengl/GPUProgramParameter.h"
#include "graphics/opengl/GraphicsSystem.h"
#include "graphics/opengl/Mesh.h"
#include "graphics/opengl/Texture.h"
#include "graphics/opengl/TextureCube.h"
#include "resource_manager/ResourceManager.h"
#include <string>
#include <utility>
#include <vector>

namespace Nova
{
RendererBackendDeferred::RendererBackendDeferred()
    : mCurrentSkybox(ResourceManager::getInstance().get<PBRSkybox>("textures/skyboxes/studio")),
      mGBuffer(FrameBuffer::makeGBuffer(mWidth, mHeight)),
      mLightPassFrameBuffer(FrameBuffer::makePostProcessFrameBuffer(mWidth, mHeight)),
      mHBloomFrameBuffer(FrameBuffer::makePostProcessFrameBuffer(mWidth / 4, mHeight / 4)),
      mVBloomFrameBuffer(FrameBuffer::makePostProcessFrameBuffer(mWidth / 4, mHeight / 4)),

      mLightPassRenderPacket(
          mScreenQuad,
          std::make_shared<Material>(
              ResourceManager::getInstance().get<GPUProgram>("shaders/light_pass_PBR"),
              std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                  {mGBuffer.getColorTexture(0),
                   "uGPosMetal"}, // gBuffer position and metallic channel
                  {mGBuffer.getColorTexture(1),
                   "uGNormRough"}, // gBuffer normal and roughness channel
                  {mGBuffer.getColorTexture(2), "uGAlbedoSkyboxmask"}, // Albedo and skybox mask
                  {mGBuffer.getColorTexture(3), "uGNormalMapAO"},
                  {mCurrentSkybox->irradiance, "uIrradianceMap"},
                  {mCurrentSkybox->radiance, "uRadianceMap"},
                  {mCurrentSkybox->BRDFLUT, "uBRDFLUT"}})),

      mFinalPacket(mScreenQuad,
                   std::make_shared<Material>(
                       ResourceManager::getInstance().get<GPUProgram>("shaders/tonemap"),
                       std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                           {mVBloomFrameBuffer.getColorTexture(0), "uBlurredImage"},
                           {mLightPassFrameBuffer.getColorTexture(0), "uRenderedImage"},
                       })),

      mHBloomPacket(mScreenQuad,
                    std::make_shared<Material>(
                        ResourceManager::getInstance().get<GPUProgram>("shaders/hbloom"),
                        std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                            {mLightPassFrameBuffer.getColorTexture(0), "uAlbedo"}})),

      mVBloomPacket(mScreenQuad,
                    std::make_shared<Material>(
                        ResourceManager::getInstance().get<GPUProgram>("shaders/vbloom"),
                        std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                            {mHBloomFrameBuffer.getColorTexture(0), "uAlbedo"}})),

      mCurrentSkyBoxPacket(std::make_shared<Mesh>(Mesh::makeSkyBoxMesh()),
                           std::make_shared<Material>(
                               ResourceManager::getInstance().get<GPUProgram>("shaders/skybox"),
                               std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                                   {mCurrentSkybox->texture, "skyboxTexture"}}))
{
}

void RendererBackendDeferred::init() {}

void RendererBackendDeferred::render()
{
    // glEnable(GL_SCISSOR_TEST);
    // glViewport(0, 0, Window::getInstance().getWidth(), Window::getInstance().getHeight());
    // glScissor(0, 0, Window::getInstance().getWidth(), Window::getInstance().getHeight());
    // GraphicsSystem::getInstance().setWireframeMode(true);
    Timer timer;
    // GEOMETRY PASS
    mGBuffer.bind();
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);
    /*glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);*/

    auto &renderPackets = GraphicsSystem::getInstance().getRenderPackets();
    for (const auto &packet : renderPackets) // for all drawable objects in the world
    {
        packet.second.bind();
        packet.second.updateAllUniforms();
        packet.second.draw();
        // packet.unBind();
    }
    /*glStencilMask(0x00);*/
    auto shader = ResourceManager::getInstance().get<GPUProgram>("shaders/physicsDebugDraw");
    shader->bind();
    PhysicsDebugDraw();
    //shader->unBind();

    mCurrentSkyBoxPacket.bind();
    mCurrentSkyBoxPacket.updateAllUniforms();
    mCurrentSkyBoxPacket.draw();

    // mGBuffer.unBind();
    mProfileTimes["Geometry pass"] = timer.getMicro();
    timer.reset();

    // LIGHTING PASS
    glDisable(GL_DEPTH_TEST);
    // glDepthMask(0x00);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mLightPassFrameBuffer.bind();
    {
        mLightPassRenderPacket.bind();
        glUniform1i(glGetUniformLocation(
                        mLightPassRenderPacket.getMaterial()->getGPUProgram()->getProgramID(),
                        "uCurrentMaxLights"),
                    GraphicsSystem::getInstance().currentNumLights);
        mLightPassRenderPacket.updateAllUniforms();
        mLightPassRenderPacket.draw();
        mLightPassFrameBuffer.getColorTexture(0)->bind();
        glGenerateMipmap(GL_TEXTURE_2D);
        // mLightPassFrameBuffer.getColorTexture(0)->unBind();
    }
    // mLightPassFrameBuffer.unBind();
    mProfileTimes["Light pass"] = timer.getMicro();
    timer.reset();

    // POST-PROCESS
    glViewport(0, 0, mWidth / 4, mHeight / 4);
    mHBloomFrameBuffer.bind();
    mHBloomPacket.bind();
    mHBloomPacket.draw();
    mHBloomFrameBuffer.getColorTexture(0)->bind();
    glGenerateMipmap(GL_TEXTURE_2D);
    // mHBloomFrameBuffer.getColorTexture(0)->unBind();

    mVBloomFrameBuffer.getColorTexture(0)->bind();
    glGenerateMipmap(GL_TEXTURE_2D);
    // mVBloomFrameBuffer.getColorTexture(0)->unBind();
    mVBloomFrameBuffer.bind();
    mVBloomPacket.bind();
    mVBloomPacket.draw();
    mVBloomFrameBuffer.unBind();

    // WRITES TO BACKBUFFER
    glViewport(0, 0, mWidth, mHeight);
    mFinalPacket.bind();
    mFinalPacket.draw();
    mFinalPacket.unBind(); // why do i have to unbind this exactly?

    // glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    // glDepthMask(0xFF);
    // renderPackets.clear();
    // lights.clear();
    mProfileTimes["Post-process"] = timer.getMicro();
    timer.reset();

    // Window::getInstance().setTitle("geometry pass:" + std::to_string(mGPassTime) +
    //                                " us. | light pass:" + std::to_string(mLightPassTime) +
    //                                " us. | postprocess pass:" +
    //                                std::to_string(mPostprocessTime));
    DebugUI::getInstance().drawGUI();
}

void RendererBackendDeferred::setSkyBox(const std::shared_ptr<PBRSkybox> &skybox)
{
    if (!skybox)
    {
        LOG_ERROR("can't set skybox, the skybox object is invalid");
        return;
    }
    mCurrentSkybox = skybox;

    std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> v1{
        {mCurrentSkybox->texture, "uSkyboxTexture"}};
    mCurrentSkyBoxPacket.getMaterial()->setTextures(v1);

    std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> v2{
        {mGBuffer.getColorTexture(0), "gPosMetal"},         // gBuffer position and metallic channel
        {mGBuffer.getColorTexture(1), "gNormRough"},        // gBuffer normal and roughness channel
        {mGBuffer.getColorTexture(2), "gAlbedoSkyboxmask"}, // gBuffer Albedo and (skybox
                                                            // mask)ambient occlussion channel
        {mGBuffer.getColorTexture(3), "gNormalMapAO"},
        {mCurrentSkybox->irradiance, "uIrradianceMap"},
        {mCurrentSkybox->radiance, "uRadianceMap"},
        {mCurrentSkybox->BRDFLUT, "uBRDFLUT"}};
    mLightPassRenderPacket.getMaterial()->setTextures(v2);
}

void RendererBackendDeferred::drawLine(const Vec3 &from, const Vec3 &to, const Vec3 &color)
{
    auto shader = ResourceManager::getInstance().get<GPUProgram>("shaders/physicsDebugDraw");    
    auto id = shader->getProgramID();
    auto &camera = GraphicsSystem::getInstance().getCurrentCamera();
    glUniformMatrix4fv(glGetUniformLocation(id, "uView"), 1, GL_FALSE, camera.view->getDataPtr());
    glUniformMatrix4fv(glGetUniformLocation(id, "uProj"), 1, GL_FALSE,
                       camera.projection->getDataPtr());

    Vec3 vColor(0.0f,1.0f,0.0f);
    glUniform3fv(glGetUniformLocation(id, "uColor"), 1, vColor.getDataPtr());

    GLfloat vertices[] = {from.getX(), from.getY(), from.getZ(), to.getX(), to.getY(), to.getZ()};

    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); // bind the VAO
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0,        // same as in glEnableVertexAttribArray(0)
                          3,        // # of coordinates that build a vertex
                          GL_FLOAT, // data type
                          GL_FALSE, // normalized?
                          0,        // stride
                          nullptr); // vbo offset

    glDrawArrays(GL_LINES, 0, 2);
    glDisableVertexAttribArray(0);

    glBindVertexArray(0);

    glDeleteVertexArrays(1, &vao); //this is a bad idea
}

} // namespace Nova