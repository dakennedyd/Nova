#include <functional>
#include <memory>
#include "ResourceManager.h"
#include "logger/Logger.h"
#include "Timer.h"
#include "resource_manager/ResourceBase.h"
#include "Xml.h"
#include "resource_manager/DefaultLoaders.h"

namespace Nova {	

	void ResourceManager::startUp()
	{
		Timer timer;
		LOG_INFO("Initiating the Resource Manager");

		/* registers loader functions for each resource supported */
		std::function < std::shared_ptr<ResourceBase>(const XMLNode &)> loadMaterialFunction = loadMaterial;
		registerLoader("material", loadMaterialFunction);
		std::function < std::shared_ptr<ResourceBase>(const XMLNode &)> loadMeshFunction = loadMesh;
		registerLoader("mesh", loadMeshFunction);
		std::function < std::shared_ptr<ResourceBase>(const XMLNode &)> loadTextureFunction = loadTexture;
		registerLoader("texture", loadTextureFunction);
		std::function < std::shared_ptr<ResourceBase>(const XMLNode &)> loadShaderFunction = loadShader;
		registerLoader("program", loadShaderFunction);

		/*FileSystem& fs = FileSystem::getInstance();
		auto fileNames = fs.getFilenamesInDirectory(TEXTURES_PATH + "skyboxes/", true);
		for (auto fileName : fileNames) { registerResource<TextureCube>(fileName, fileName);}

		fileNames = fs.getFilenamesInDirectory(TEXTURES_PATH, true);
		for (auto fileName : fileNames) { registerResource<Texture>(fileName, fileName); }

		fileNames = fs.getFilenamesInDirectory(SHADERS_PATH, true);
		for (auto fileName : fileNames) { registerResource<GPUProgram>(fileName, fileName); }*/
		
		/* loads resources from resource index file 
		TODO: this should be extensible */
		XMLNode resource(mXMLFile.getRootElement());
		resource = resource.getFirstChild();
		while (!resource.isEmpty())
		{
			XMLNode node(resource.getFirstChild());			
			while (!node.isEmpty())
			{
				registerResource(node);
				node = node.getNextElement();
			}
			resource = resource.getNextElement();
		}
		LOG_INFO("Resource Manager initialized:" << mResourcesRegistered << " resources registered in " << timer.getMillis() << "ms.");
	}
	void ResourceManager::registerLoader(const std::string & name, std::function<std::shared_ptr<ResourceBase>(const XMLNode &)> loader)
	{
		//mLoaders[name] = loader;
		mLoaders.emplace(std::make_pair(name, loader));
	}
}