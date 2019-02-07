
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
      mHBloomFrameBuffer{{FrameBuffer::makePostProcessFrameBuffer(mWidth / 2, mHeight / 2)},
                         {FrameBuffer::makePostProcessFrameBuffer(mWidth / 3, mHeight / 3)},
                         {FrameBuffer::makePostProcessFrameBuffer(mWidth / 4, mHeight / 4)},
                         {FrameBuffer::makePostProcessFrameBuffer(mWidth / 5, mHeight / 5)},
                         {FrameBuffer::makePostProcessFrameBuffer(mWidth / 6, mHeight / 6)}},

      mVBloomFrameBuffer{{FrameBuffer::makePostProcessFrameBuffer(mWidth / 2, mHeight / 2)},
                         {FrameBuffer::makePostProcessFrameBuffer(mWidth / 3, mHeight / 3)},
                         {FrameBuffer::makePostProcessFrameBuffer(mWidth / 4, mHeight / 4)},
                         {FrameBuffer::makePostProcessFrameBuffer(mWidth / 5, mHeight / 5)},
                         {FrameBuffer::makePostProcessFrameBuffer(mWidth / 6, mHeight / 6)}},

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

      mHBloomPacket{
          {mScreenQuad, std::make_shared<Material>(
                            ResourceManager::getInstance().get<GPUProgram>("shaders/hbloom"),
                            std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                                {mLightPassFrameBuffer.getColorTexture(0), "uAlbedo"}})},
          {mScreenQuad, std::make_shared<Material>(
                            ResourceManager::getInstance().get<GPUProgram>("shaders/hbloom"),
                            std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                                {mVBloomFrameBuffer[0].getColorTexture(0), "uAlbedo"}})},
          {mScreenQuad, std::make_shared<Material>(
                            ResourceManager::getInstance().get<GPUProgram>("shaders/hbloom"),
                            std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                                {mVBloomFrameBuffer[1].getColorTexture(0), "uAlbedo"}})},
          {mScreenQuad, std::make_shared<Material>(
                            ResourceManager::getInstance().get<GPUProgram>("shaders/hbloom"),
                            std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                                {mVBloomFrameBuffer[2].getColorTexture(0), "uAlbedo"}})},
          {mScreenQuad, std::make_shared<Material>(
                            ResourceManager::getInstance().get<GPUProgram>("shaders/hbloom"),
                            std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                                {mVBloomFrameBuffer[3].getColorTexture(0), "uAlbedo"}})},
      },

      mVBloomPacket{
          {mScreenQuad, std::make_shared<Material>(
                            ResourceManager::getInstance().get<GPUProgram>("shaders/vbloom"),
                            std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                                {mHBloomFrameBuffer[0].getColorTexture(0), "uAlbedo"}})},
          {mScreenQuad, std::make_shared<Material>(
                            ResourceManager::getInstance().get<GPUProgram>("shaders/vbloom"),
                            std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                                {mHBloomFrameBuffer[1].getColorTexture(0), "uAlbedo"}})},
          {mScreenQuad, std::make_shared<Material>(
                            ResourceManager::getInstance().get<GPUProgram>("shaders/vbloom"),
                            std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                                {mHBloomFrameBuffer[2].getColorTexture(0), "uAlbedo"}})},
          {mScreenQuad, std::make_shared<Material>(
                            ResourceManager::getInstance().get<GPUProgram>("shaders/vbloom"),
                            std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                                {mHBloomFrameBuffer[3].getColorTexture(0), "uAlbedo"}})},
          {mScreenQuad, std::make_shared<Material>(
                            ResourceManager::getInstance().get<GPUProgram>("shaders/vbloom"),
                            std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                                {mHBloomFrameBuffer[4].getColorTexture(0), "uAlbedo"}})},
      },

      mCurrentSkyBoxPacket(
          std::make_shared<Mesh>(Mesh::makeSkyBoxMesh()),
          std::make_shared<Material>(
              ResourceManager::getInstance().get<GPUProgram>("shaders/skybox_deferred_pbr"),
              std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                  {mCurrentSkybox->texture, "uSkyboxTexture"}})),
      mFinalPacket(mScreenQuad,
                   std::make_shared<Material>(
                       ResourceManager::getInstance().get<GPUProgram>("shaders/tonemap"),
                       std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                           {mVBloomFrameBuffer[4].getColorTexture(0), "uBlurredImage"},
                           {mLightPassFrameBuffer.getColorTexture(0), "uRenderedImage"},
                       }))
{
    // mLightPassFrameBuffer.attachTexture(mGBuffer.getDepthBuffer());

    mLightPassFrameBuffer.attachTexture(
        std::make_shared<Texture>(mWidth, mHeight, nullptr, TextureType::DEPTHBUFFER,
                                  Filtering::NEAREST, MipMapping::NO_MIPMAP));
    if (!mLightPassFrameBuffer.isComplete())
    {
        LOG_ERROR("can't attach depth buffer to lightpass framebuffer"); // check for framebuffer
                                                                         // completeness
    }
}

void RendererBackendDeferred::init() {}

void RendererBackendDeferred::updateLights()
{
    auto id = mLightPassRenderPacket.getMaterial()->getGPUProgram()->getProgramID();
    auto &lightsList = GraphicsSystem::getInstance().getLights();
    std::size_t i = 0;
    for (auto &light : lightsList)
    {
        if (i >= MAX_LIGHTS) break;
        glUniform3fv(
            glGetUniformLocation(id, ("uLights[" + std::to_string(i) + "].position").c_str()), 1,
            light.second.getPosition()->getDataPtr());
        glUniform3fv(glGetUniformLocation(id, ("uLights[" + std::to_string(i) + "].color").c_str()),
                     1, light.second.getColor()->getDataPtr());
        glUniform1i(glGetUniformLocation(id, ("uLights[" + std::to_string(i) + "].type").c_str()),
                    light.second.getTypeCode());
        i++;
    }
}

void RendererBackendDeferred::render()
{
    // glEnable(GL_SCISSOR_TEST);
    // glViewport(0, 0, Window::getInstance().getWidth(), Window::getInstance().getHeight());
    // glScissor(0, 0, Window::getInstance().getWidth(), Window::getInstance().getHeight());
    // GraphicsSystem::getInstance().setWireframeMode(true);
    Timer timer;
    // ==============================GEOMETRY PASS========================================
    mGBuffer.bind();
    {
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);
        /*glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);*/

        auto &renderPackets = GraphicsSystem::getInstance().getRenderPackets();
        for (const auto &packet : renderPackets) // for all drawable objects in the world
        {
            packet.second.bind();
            packet.second.updateCamera();
            packet.second.updateAllUniforms();
            packet.second.draw();
            // packet.second.unBind();
        }
        /*glStencilMask(0x00);*/

        //=======DEBUG DRAW=========
        // auto shader = ResourceManager::getInstance().get<GPUProgram>("shaders/physicsDebugDraw");
        // shader->bind();
        // PhysicsDebugDraw();
        // shader->unBind();

        // mGBuffer.unBind();
        mProfileTimes["Geometry pass"] = timer.getMicro();
        timer.reset();
    }
    //=================================LIGHTING PASS====================================

    // glDepthMask(0x00);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glDisable(GL_DEPTH_TEST);

    mLightPassFrameBuffer.bind();
    {
        glClear(/*GL_COLOR_BUFFER_BIT |*/ GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);
        // glDepthMask(GL_FALSE);
        // glDepthMask(GL_TRUE);

        mLightPassRenderPacket.bind();
        // mLightPassRenderPacket.setUniform("uCurrentMaxLights",
        // static_cast<int>(GraphicsSystem::getInstance().currentNumLights));
        updateLights();
        mLightPassRenderPacket.updateCamera();
        // mLightPassRenderPacket.updateAllUniforms();
        mLightPassRenderPacket.draw();
        // mLightPassRenderPacket.unBind();

        glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer.getFrameBufferID());
        //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mLightPassFrameBuffer.getFrameBufferID());
        glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_DEPTH_BUFFER_BIT,
                          GL_NEAREST);
        // glBindFramebuffer(GL_READ_FRAMEBUFFER, mLightPassFrameBuffer.getFrameBufferID());
        mCurrentSkyBoxPacket.bind();
        mCurrentSkyBoxPacket.updateCamera();
        // mCurrentSkyBoxPacket.updateAllUniforms();
        mCurrentSkyBoxPacket.draw();

        // mLightPassFrameBuffer.getColorTexture(0)->bind();
        // glGenerateMipmap(GL_TEXTURE_2D);
        // mLightPassFrameBuffer.getColorTexture(0)->unBind();
    }
    // glEnable(GL_DEPTH_TEST);

    // mLightPassFrameBuffer.unBind();
    mProfileTimes["Light pass"] = timer.getMicro();
    timer.reset();
    //===============================POST-PROCESS====================================================
    int i = 0;
    for (; i < 5; i++)
    {
        glViewport(0, 0, mWidth / (i + 2), mHeight / (i + 2));
        mHBloomFrameBuffer[i].bind();
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        mHBloomPacket[i].bind();
        mHBloomPacket[i].draw();
        // mHBloomFrameBuffer[i].getColorTexture(0)->bind();
        // glGenerateMipmap(GL_TEXTURE_2D);
        // mHBloomFrameBuffer[i].getColorTexture(0)->unBind();

        mVBloomFrameBuffer[i].bind();
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        mVBloomPacket[i].bind();
        mVBloomPacket[i].draw();
        // mVBloomFrameBuffer[i].getColorTexture(0)->bind();
        // glGenerateMipmap(GL_TEXTURE_2D);
        // mVBloomFrameBuffer[i].getColorTexture(0)->unBind();
    }
    mVBloomFrameBuffer[i - 1].unBind();
    // WRITES TO BACKBUFFER
    glViewport(0, 0, mWidth, mHeight);
    mFinalPacket.bind();
    // mFinalPacket.updateAllUniforms();
    mFinalPacket.setUniform("uTime", Timer::getTimeSinceEngineStart() / 1000.0f);
    mFinalPacket.draw();
    mFinalPacket.unBind(); // why is this necesarry?

    // glDisable(GL_BLEND);
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
        {mGBuffer.getColorTexture(0), "uGPosMetal"},  // gBuffer position and metallic channel
        {mGBuffer.getColorTexture(1), "uGNormRough"}, // gBuffer normal and roughness channel
        {mGBuffer.getColorTexture(2), "uGAlbedoSkyboxmask"}, // gBuffer Albedo and (skybox
                                                             // mask)ambient occlussion channel
        {mGBuffer.getColorTexture(3), "uGNormalMapAO"},
        {mCurrentSkybox->irradiance, "uIrradianceMap"},
        {mCurrentSkybox->radiance, "uRadianceMap"},
        {mCurrentSkybox->BRDFLUT, "uBRDFLUT"}};
    mLightPassRenderPacket.getMaterial()->setTextures(v2);
}

// this function is a disaster
void RendererBackendDeferred::drawLine(const Vec3 &from, const Vec3 &to, const Vec3 &color)
{
    auto shader = ResourceManager::getInstance().get<GPUProgram>("shaders/physicsDebugDraw");
    auto id = shader->getProgramID();
    auto &camera = GraphicsSystem::getInstance().getCurrentCamera();
    glUniformMatrix4fv(glGetUniformLocation(id, "uView"), 1, GL_FALSE,
                       camera.view->getDataPtr());
    glUniformMatrix4fv(glGetUniformLocation(id, "uProj"), 1, GL_FALSE,
                       camera.projection->getDataPtr());

    Vec3 vColor(0.0f, 1.0f, 0.0f);
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

    glDeleteVertexArrays(1, &vao); // this is a bad idea
}

void RendererBackendDeferred::addLight()
{
    size_t numLights = GraphicsSystem::getInstance().getNumLights();
    if (numLights <= MAX_LIGHTS && numLights > 0)
    {
        LOG_DEBUG("adding light:" << GraphicsSystem::getInstance().getNumLights());
        mLightPassRenderPacket.getMaterial()->getGPUProgram()->recompile(
            {{ShaderType::FRAGMENT_SHADER,
              "#define MAX_LIGHTS " + std::to_string(numLights) + "\n"}});
    }
    else if (numLights < 1)
    {
        mLightPassRenderPacket.getMaterial()->getGPUProgram()->recompile();
    }
}

void RendererBackendDeferred::removeLight() { addLight(); }

} // namespace Nova