#pragma once
//#include "IRendererFrontend.h"

namespace Nova {
	class RendererFrontend :public IRendererFrontend
	{
	public:
		RendererFrontend();
		~RendererFrontend() = default;
		inline void createRenderPackets() override
		{
			//auto lightSystem = Application::getInstance().getWorld().GetSystem<LightSystem>();
			//auto &lightEntities = lightSystem->getEntities();
			auto &graphicsSystem = GraphicsSystem::getInstance();
			//for (auto &it : lightEntities)//for each entity that produces light
			//{
			//	auto& lc = it.second->GetComponent<LightComponent>();
			//	//int type = 1;
			//	//if (lc.type == LightType::POINT_LIGHT) type = 1;
			//	//else if(lc.type == LightType::DIRECTIONAL_LIGHT) type = 0;
			//	graphicsSystem.addLight(Light{ lc.type,
			//		&(it.second->getNonConstTransformStruct().finalTranslation), &(lc.ambientColor), &(lc.diffuseColor),
			//		&(lc.specularColor) });
			//}

			auto visualSystem = Application::getInstance().getWorld().GetSystem<VisualSystem>();
			auto &visualEntities = visualSystem->getEntities();
			for (auto &it : visualEntities)//for each renderable entity
			{
				auto vc = it.second->GetComponent<VisualComponent>();
				graphicsSystem.addPacket(RenderPacket{ vc.mesh, vc.material, &it.second->getTransformStruct() });
			}
		}
	};
}
