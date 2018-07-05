#pragma once

namespace Nova {
	struct Skybox;
	class IRendererBackend
	{
	public:
		IRendererBackend() = default;
		virtual ~IRendererBackend() = default;
		virtual void render() = 0;
		virtual void setSkyBox(const Skybox &skyBox) = 0;
		//virtual void setSkyBox(std::shared_ptr<TextureCube> skyBox) = 0;
		//virtual void setIBLData(std::shared_ptr<IBL_Data> data) = 0; NOO
		//virtual std::uint64_t addLight(ILight light) = 0;
		//virtual std::uint64_t addPacket(IRenderPacket packet) = 0;
		/*note: parameter is not a reference becuse RenderPacket is not copyable
		so insted uses the move constructor*/
		//virtual std::uint64_t addPacket(RenderPacket primitive) = 0;
		//virtual void setSkyBox(RenderPacket *skyBox) = 0;
	};

}