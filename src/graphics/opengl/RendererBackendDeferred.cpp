#include <memory>
#include <vector>
#include "RendererBackendDeferred.h"
#include "graphics/opengl/FrameBuffer.h"
#include "resource_manager/ResourceManager.h"
#include "graphics/opengl/GraphicsSystem.h"
#include "graphics/opengl/GPUProgram.h"
#include "graphics/opengl/GPUProgramParameter.h"
#include "graphics/opengl/Mesh.h"
#include "graphics/opengl/Texture.h"

namespace Nova {
	RendererBackendDeferred::RendererBackendDeferred()
		:mGBuffer(FrameBuffer::makeGBuffer(mWidth, mHeight)),
		mLightPassFrameBuffer(FrameBuffer::makePostProcessFrameBuffer(mWidth, mHeight)),
		mIBL(ResourceManager::getInstance().get<IBL_Data>("fireSky_IBL")->irradiance,
			ResourceManager::getInstance().get<IBL_Data>("fireSky_IBL")->radiance),
		mLightPassRenderPacket(mScreenQuad,
			std::make_shared<Material>(ResourceManager::getInstance().get<GPUProgram>("light_pass_PBR"),
				std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> {
					{mGBuffer.getColorTexture(0), "gPosMetal"}, //gBuffer position and metallic channel
					{mGBuffer.getColorTexture(1), "gNormRough" }, //gBuffer normal and roughness channel
					{mGBuffer.getColorTexture(2), "gAlbedoSkyboxmask" }, //Albedo and skybox mask
					{mGBuffer.getColorTexture(3), "gNormalMapAO" },
					{mIBL.irradiance, "irradianceMap" },
					{mIBL.radiance, "radianceMap" },
					{ResourceManager::getInstance().get<Texture>("brdf_LUT"), "brdfLUT" }})),
		mCurrentSkyBox(std::make_shared<Mesh>(Mesh::makeSkyBoxMesh()),
			std::make_shared<Material>(ResourceManager::getInstance().get<GPUProgram>("skybox"),
				std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> {
					{ResourceManager::getInstance().get<TextureCube>("fireSky_skybox"), "skyboxTexture"} })),
		mHBloomFrameBuffer(FrameBuffer::makePostProcessFrameBuffer(mWidth/4 , mHeight/4)),
		mVBloomFrameBuffer(FrameBuffer::makePostProcessFrameBuffer(mWidth/4 , mHeight/4)),
		mHBloomPacket(mScreenQuad, std::make_shared<Material>(ResourceManager::getInstance().get<GPUProgram>("hbloom_shader"),
			std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> {
				{mLightPassFrameBuffer.getColorTexture(0), "albedo"} })),
		mVBloomPacket(mScreenQuad, std::make_shared<Material>(ResourceManager::getInstance().get<GPUProgram>("vbloom_shader"),
			std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> {
				{mHBloomFrameBuffer.getColorTexture(0), "albedo"} })),
		mFinalPacket(mScreenQuad,
		std::make_shared<Material>(ResourceManager::getInstance().get<GPUProgram>("tonemap_shader"),
			std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> {
				{mVBloomFrameBuffer.getColorTexture(0), "blurredImage"},
				{mLightPassFrameBuffer.getColorTexture(0), "renderedImage" }, }))
	{
		/*auto skybox = std::make_shared<RenderPacket>(std::make_shared<Mesh>(Mesh::makeSkyBoxMesh()),
			std::make_shared<Material>(ResourceManager::getInstance().get<GPUProgram>("skybox"),
				std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> {
					{ResourceManager::getInstance().get<TextureCube>("city_night01_skybox"), "albedoMap"} }));
		setSkyBox(skybox);*/
		//sets the cameraPos uniform for mLightPassRenderPacket
		auto id = ResourceManager::getInstance().get<GPUProgram>("light_pass_PBR")->getProgramID();
		auto& camera = GraphicsSystem::getInstance().getCurrentCamera();
		mLightPassRenderPacket.addParameter(
			new GPUProgramParameterVec3(glGetUniformLocation(id, "cameraPos"), camera.position->getDataPtr()));
		
		//setSkyBox(ResourceManager::getInstance().get<TextureCube>("fireSky_skybox"));
		//mCurrentSkyBox->setMaterial(ResourceManager::getInstance().get<Material>("skybox_material"));
	}
	void RendererBackendDeferred::render()
	{
		auto &renderPackets = GraphicsSystem::getInstance().getRenderPackets();
		//auto &lights = GraphicsSystem::getInstance().getLights();
		//auto& rm = ResourceManager::getInstance();
		
		/*auto &rm = ResourceManager::getInstance();
		auto GeometryPassShader = rm.get<GPUProgram>("geometry_pass");*/
						
		//glEnable(GL_SCISSOR_TEST);
		//glViewport(0, 0, Window::getInstance().getWidth(), Window::getInstance().getHeight());
		//glScissor(0, 0, Window::getInstance().getWidth(), Window::getInstance().getHeight());

		//GraphicsSystem::getInstance().setWireframeMode(true);
		//geometry pass
		mGBuffer.bind();		
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);
		/*glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);*/
		for (const auto & packet : renderPackets)//for all drawable objects in the world
		{
			packet.bind();
			packet.updateAllUniforms();
			packet.draw();
			//packet.unBind();
		}
		/*glStencilMask(0x00);*/

		mCurrentSkyBox.bind();
		mCurrentSkyBox.updateAllUniforms();
		mCurrentSkyBox.draw();
		

		//mGBuffer.unBind();
		//GraphicsSystem::getInstance().setWireframeMode(false);

		//lighting pass
		glDisable(GL_DEPTH_TEST);
		//glDepthMask(0x00);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mLightPassFrameBuffer.bind();
		{
			mLightPassFrameBuffer.getColorTexture(0)->bind();
			glGenerateMipmap(GL_TEXTURE_2D);
			mLightPassFrameBuffer.getColorTexture(0)->unBind();
			mLightPassRenderPacket.bind();
			mLightPassRenderPacket.updateAllUniforms();
			mLightPassRenderPacket.draw();
		}
		//mLightPassFrameBuffer.unBind();

		glViewport(0, 0, mWidth/4, mHeight/4);
		mHBloomFrameBuffer.getColorTexture(0)->bind();
		glGenerateMipmap(GL_TEXTURE_2D);
		mHBloomFrameBuffer.getColorTexture(0)->unBind();
		mHBloomFrameBuffer.bind();
		mHBloomPacket.bind();
		mHBloomPacket.draw();

		/*mVBloomFrameBuffer.getColorTexture(0)->bind();
		glGenerateMipmap(GL_TEXTURE_2D);
		mVBloomFrameBuffer.getColorTexture(0)->unBind();*/
		mVBloomFrameBuffer.bind();
		mVBloomPacket.bind();
		mVBloomPacket.draw();
		mVBloomFrameBuffer.unBind();

		glViewport(0, 0, mWidth, mHeight);
		mFinalPacket.bind();
		mFinalPacket.draw();

		//glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		//glDepthMask(0xFF);
		//renderPackets.clear();
		//lights.clear();
	}
	void RendererBackendDeferred::setSkyBox(const Skybox &skybox)
	{
		//ResourceManager::getInstance().get<Material>("skybox_material")->ge
		//mCurrentSkyBox->setMaterial(ResourceManager::getInstance().get<Material>("garden_skybox"));
		std::vector<std::pair< std::shared_ptr<ITexture>, std::string> > texturesVector{ {skybox.skyboxTexture, "skyboxTexture"} };
		mCurrentSkyBox.getMaterial()->setTextures(texturesVector);
		/*auto skybox = std::make_shared<RenderPacket>(std::make_shared<Mesh>(Mesh::makeSkyBoxMesh()),
			std::make_shared<Material>(ResourceManager::getInstance().get<GPUProgram>("skybox"),
				std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> {
					{ResourceManager::getInstance().get<TextureCube>("fireSky_skybox"), "albedoMap"} }));	
		GraphicsSystem::getInstance().addPacket(*skybox);*/
		//mCurrentSkyBox = skybox;		
		setIBLData(skybox.iblData);
	}
	void RendererBackendDeferred::setIBLData(std::shared_ptr<IBL_Data> data)
	{
		mIBL.radiance = data->radiance;
		mIBL.irradiance = data->irradiance;
		std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> texturesVector{
			{mGBuffer.getColorTexture(0), "gPosMetal"}, //gBuffer position and metallic channel
			{ mGBuffer.getColorTexture(1), "gNormRough" }, //gBuffer normal and roughness channel
			{ mGBuffer.getColorTexture(2), "gAlbedoSkyboxmask" }, //gBuffer Albedo and (skybox mask)ambient occlussion channel
			{ mGBuffer.getColorTexture(3), "gNormalMapAO" },
			{ mIBL.irradiance, "irradianceMap" },
			{ mIBL.radiance, "prefilterMap" },
			{ ResourceManager::getInstance().get<Texture>("brdf_LUT"), "brdfLUT" } };
		mLightPassRenderPacket.getMaterial()->setTextures(texturesVector);
	}
}