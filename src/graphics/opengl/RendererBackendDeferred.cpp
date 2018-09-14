
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
    : mIBL(
          ResourceManager::getInstance().get<IBL_Data>("textures/skyboxes/fireSky_IBL")->irradiance,
          ResourceManager::getInstance().get<IBL_Data>("textures/skyboxes/fireSky_IBL")->radiance),

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
                  {mIBL.irradiance, "uIrradianceMap"},
                  {mIBL.radiance, "uRadianceMap"},
                  {ResourceManager::getInstance().get<Texture>("textures/brdf_LUT"), "uBRDFLUT"}})),

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

      mCurrentSkyBox(
          std::make_shared<Mesh>(Mesh::makeSkyBoxMesh()),
          std::make_shared<Material>(
              ResourceManager::getInstance().get<GPUProgram>("shaders/skybox"),
              std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                  {ResourceManager::getInstance().get<TextureCube>("textures/skyboxes/fireSky"),
                   "skyboxTexture"}}))
{
    mProfileTimes.push_back(std::make_pair("Geometry pass", 0));
    mProfileTimes.push_back(std::make_pair("Light pass", 0));
    mProfileTimes.push_back(std::make_pair("Post-process", 0));
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

    mCurrentSkyBox.bind();
    mCurrentSkyBox.updateAllUniforms();
    mCurrentSkyBox.draw();
    // mGBuffer.unBind();
    mProfileTimes[0].second = timer.getMicro();
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
    mProfileTimes[1].second = timer.getMicro();
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

    // glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    // glDepthMask(0xFF);
    // renderPackets.clear();
    // lights.clear();
    mProfileTimes[2].second = timer.getMicro();
    timer.reset();

    // Window::getInstance().setTitle("geometry pass:" + std::to_string(mGPassTime) +
    //                                " us. | light pass:" + std::to_string(mLightPassTime) +
    //                                " us. | postprocess pass:" +
    //                                std::to_string(mPostprocessTime));
}

void RendererBackendDeferred::setSkyBox(const Skybox &skybox)
{
    // ResourceManager::getInstance().get<Material>("skybox_material")->ge
    // mCurrentSkyBox->setMaterial(ResourceManager::getInstance().get<Material>("garden_skybox"));
    std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> texturesVector{
        {skybox.skyboxTexture, "uSkyboxTexture"}};
    mCurrentSkyBox.getMaterial()->setTextures(texturesVector);
    /*auto skybox = std::make_shared<RenderPacket>(std::make_shared<Mesh>(Mesh::makeSkyBoxMesh()),
            std::make_shared<Material>(ResourceManager::getInstance().get<GPUProgram>("skybox"),
                    std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> {
                            {ResourceManager::getInstance().get<TextureCube>("fireSky_skybox"),
    "albedoMap"} })); GraphicsSystem::getInstance().addPacket(*skybox);*/
    // mCurrentSkyBox = skybox;
    setIBLData(skybox.iblData);
}

void RendererBackendDeferred::setIBLData(std::shared_ptr<IBL_Data> data)
{
    mIBL.radiance = data->radiance;
    mIBL.irradiance = data->irradiance;
    std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> texturesVector{
        {mGBuffer.getColorTexture(0), "gPosMetal"},  // gBuffer position and metallic channel
        {mGBuffer.getColorTexture(1), "gNormRough"}, // gBuffer normal and roughness channel
        {mGBuffer.getColorTexture(2),
         "gAlbedoSkyboxmask"}, // gBuffer Albedo and (skybox mask)ambient occlussion channel
        {mGBuffer.getColorTexture(3), "gNormalMapAO"},
        {mIBL.irradiance, "uIrradianceMap"},
        {mIBL.radiance, "uRadianceMap"},
        {ResourceManager::getInstance().get<Texture>("textures/brdf_LUT"), "uBRDFLUT"}};
    mLightPassRenderPacket.getMaterial()->setTextures(texturesVector);
}

} // namespace Nova