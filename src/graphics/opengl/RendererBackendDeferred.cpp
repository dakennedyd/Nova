
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
    : mIBL(ResourceManager::getInstance().get<IBL_Data>("fireSky_IBL")->irradiance,
           ResourceManager::getInstance().get<IBL_Data>("fireSky_IBL")->radiance),

      mGBuffer(FrameBuffer::makeGBuffer(mWidth, mHeight)),

      mLightPassFrameBuffer(FrameBuffer::makePostProcessFrameBuffer(mWidth, mHeight)),

      mHBloomFrameBuffer(FrameBuffer::makePostProcessFrameBuffer(mWidth / 4, mHeight / 4)),

      mVBloomFrameBuffer(FrameBuffer::makePostProcessFrameBuffer(mWidth / 4, mHeight / 4)),

      mLightPassRenderPacket(
          mScreenQuad,
          std::make_shared<Material>(
              ResourceManager::getInstance().get<GPUProgram>("light_pass_PBR"),
              std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                  {mGBuffer.getColorTexture(0),
                   "gPosMetal"}, // gBuffer position and metallic channel
                  {mGBuffer.getColorTexture(1),
                   "gNormRough"}, // gBuffer normal and roughness channel
                  {mGBuffer.getColorTexture(2), "gAlbedoSkyboxmask"}, // Albedo and skybox mask
                  {mGBuffer.getColorTexture(3), "gNormalMapAO"},
                  {mIBL.irradiance, "irradianceMap"},
                  {mIBL.radiance, "radianceMap"},
                  {ResourceManager::getInstance().get<Texture>("brdf_LUT"), "brdfLUT"}})),

      mFinalPacket(mScreenQuad,
                   std::make_shared<Material>(
                       ResourceManager::getInstance().get<GPUProgram>("tonemap_shader"),
                       std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                           {mVBloomFrameBuffer.getColorTexture(0), "blurredImage"},
                           {mLightPassFrameBuffer.getColorTexture(0), "renderedImage"},
                       })),

      mHBloomPacket(mScreenQuad,
                    std::make_shared<Material>(
                        ResourceManager::getInstance().get<GPUProgram>("hbloom_shader"),
                        std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                            {mLightPassFrameBuffer.getColorTexture(0), "albedo"}})),

      mVBloomPacket(mScreenQuad,
                    std::make_shared<Material>(
                        ResourceManager::getInstance().get<GPUProgram>("vbloom_shader"),
                        std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                            {mHBloomFrameBuffer.getColorTexture(0), "albedo"}})),

      mCurrentSkyBox(std::make_shared<Mesh>(Mesh::makeSkyBoxMesh()),
                     std::make_shared<Material>(
                         ResourceManager::getInstance().get<GPUProgram>("skybox"),
                         std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{
                             {ResourceManager::getInstance().get<TextureCube>("fireSky_skybox"),
                              "skyboxTexture"}}))
{
}

void RendererBackendDeferred::init() {}

void RendererBackendDeferred::render()
{
    // glEnable(GL_SCISSOR_TEST);
    // glViewport(0, 0, Window::getInstance().getWidth(), Window::getInstance().getHeight());
    // glScissor(0, 0, Window::getInstance().getWidth(), Window::getInstance().getHeight());

    // GraphicsSystem::getInstance().setWireframeMode(true);
    // geometry pass
    mGBuffer.bind();
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);
    /*glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);*/

    auto &renderPackets = GraphicsSystem::getInstance().getRenderPackets();
    for (const auto &packet : renderPackets) // for all drawable objects in the world
    {
        packet.bind();
        packet.updateAllUniforms();
        packet.draw();
        // packet.unBind();
    }
    /*glStencilMask(0x00);*/

    mCurrentSkyBox.bind();
    mCurrentSkyBox.updateAllUniforms();
    mCurrentSkyBox.draw();
    // mGBuffer.unBind();

    // lighting pass
    glDisable(GL_DEPTH_TEST);
    // glDepthMask(0x00);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mLightPassFrameBuffer.bind();
    {
        mLightPassRenderPacket.bind();
        mLightPassRenderPacket.updateAllUniforms();
        mLightPassRenderPacket.draw();
        mLightPassFrameBuffer.getColorTexture(0)->bind();
        glGenerateMipmap(GL_TEXTURE_2D);
        // mLightPassFrameBuffer.getColorTexture(0)->unBind();
    }
    // mLightPassFrameBuffer.unBind();

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

    glViewport(0, 0, mWidth, mHeight);
    mFinalPacket.bind();
    mFinalPacket.draw();

    // glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    // glDepthMask(0xFF);
    // renderPackets.clear();
    // lights.clear();
}

void RendererBackendDeferred::setSkyBox(const Skybox &skybox)
{
    // ResourceManager::getInstance().get<Material>("skybox_material")->ge
    // mCurrentSkyBox->setMaterial(ResourceManager::getInstance().get<Material>("garden_skybox"));
    std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> texturesVector{
        {skybox.skyboxTexture, "skyboxTexture"}};
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
        {mIBL.irradiance, "irradianceMap"},
        {mIBL.radiance, "prefilterMap"},
        {ResourceManager::getInstance().get<Texture>("brdf_LUT"), "brdfLUT"}};
    mLightPassRenderPacket.getMaterial()->setTextures(texturesVector);
}

} // namespace Nova