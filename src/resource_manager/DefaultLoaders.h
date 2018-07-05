#pragma once

namespace Nova {	
	std::shared_ptr<ResourceBase> loadTexture(const XMLNode &metadata);
	std::shared_ptr<ResourceBase> loadShader(const XMLNode &metadata);
	std::shared_ptr<ResourceBase> loadMaterial(const XMLNode &metadata);
	std::shared_ptr<ResourceBase> loadMesh(const XMLNode &metadata);	
}