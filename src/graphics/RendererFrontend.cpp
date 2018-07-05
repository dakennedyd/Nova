#include "RendererFrontend.h"

namespace Nova {
	RendererFrontend::RendererFrontend()
	{
		auto lightSystem = Application::getInstance().getWorld().GetSystem<LightSystem>();
		auto &lightEntities = lightSystem->getEntities();
		auto &graphicsSystem = GraphicsSystem::getInstance();
		for (auto &numberEntityPair : lightEntities)//for each entity that produces light
		{
			auto& lc = numberEntityPair.second->GetComponent<LightComponent>();
			//int type = 1;
			//if (lc.type == LightType::POINT_LIGHT) type = 1;
			//else if(lc.type == LightType::DIRECTIONAL_LIGHT) type = 0;
			graphicsSystem.addLight(Light{ lc.type,
				&(numberEntityPair.second->getNonConstTransformStruct().finalTranslation), &(lc.color) });
		}
	}
}