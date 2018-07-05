#pragma once
#include "graphics/IRendererBackend.h"

namespace Nova {
	struct IBL_Data : public ResourceBase
	{
		IBL_Data(const std::shared_ptr<TextureCube> &irradiance,
			const std::shared_ptr<TextureCube> radiance)
			:radiance(radiance), irradiance(irradiance) {};

		std::shared_ptr<TextureCube> irradiance;
		//std::vector<std::shared_ptr<TextureCube>> radiance;
		std::shared_ptr<TextureCube> radiance;
	};
	struct Skybox
	{
		std::shared_ptr<TextureCube> skyboxTexture;
		std::shared_ptr<IBL_Data> iblData;
	};

	// represents one link in the chain of events that
	// the renderer goes trhough to render a frame
	struct ChainLink
	{

	};

	class RendererBackendDeferred :public IRendererBackend
	{
	public:
		RendererBackendDeferred();
		~RendererBackendDeferred() = default;

		void render() override;
		void setSkyBox(const Skybox &skyBox) override;
		//void setSkyBox(std::shared_ptr<TextureCube> skyBox) override;
		void setIBLData(std::shared_ptr<IBL_Data> data);
	private:
		int mWidth = Window::getInstance().getWidth();
		int mHeight = Window::getInstance().getHeight();
		std::shared_ptr<Mesh> mScreenQuad = std::make_shared<Mesh>(Mesh::makeQuad(2.0f, 2.0f));
		IBL_Data mIBL;
		FrameBuffer mGBuffer;
		FrameBuffer mLightPassFrameBuffer;
		FrameBuffer mHBloomFrameBuffer;
		FrameBuffer mVBloomFrameBuffer;

		RenderPacket mLightPassRenderPacket;
		RenderPacket mFinalPacket;
		RenderPacket mHBloomPacket;
		RenderPacket mVBloomPacket;
		RenderPacket mCurrentSkyBox;
		
		//std::vector<std::pair<FrameBuffer, GPUProgram>> mFrameBuffers;
	};
}
