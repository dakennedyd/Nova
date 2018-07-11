// #pragma once
// #include "graphics/IRendererBackend.h"

// namespace Nova {
// 	class RendererBackend :public IRendererBackend//, public ISingleton<RendererBackend>
// 	{
// 		friend class GraphicSystem;

// 	public:
// 		//RendererBackend();
// 		~RendererBackend() = default;
// 		void render() override;		
		

// 	private:
// 		FrameBuffer mFrameBuffer;
// 		//std::shared_ptr<Material> mFrameBufferMaterial;
// 		//std::shared_ptr<Mesh> mQuad = nullptr;
// 		RenderPacket mFullScreenQuadPacket;
// 		//std::unordered_map<uint64_t, RenderPacket> mRenderPackets;
// 		//std::unordered_map<uint64_t, Light> mLights;
// 		//RenderPacket * mCurrentSkyBox;
// 	};

// }