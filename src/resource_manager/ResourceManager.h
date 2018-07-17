#pragma once
#include <unordered_map>
#include <memory>
#include "Xml.h"
#include "ResourceBase.h"
#include "ISingleton.h"
#include "ISubSystem.h"
#include "logger/Logger.h"
#include "Crc.h"

namespace Nova {
	struct ResourceData
	{
		std::shared_ptr<ResourceBase> resourcePointer;
		//bool isLoaded = false;
		XMLNode metaData;
		//std::shared_ptr<ResourceBase>(*load)(const XMLNode &);
		std::function<std::shared_ptr<ResourceBase>(const XMLNode &)> load;
	};
		
	class ResourceManager :public ISingleton<ResourceManager>, public ISubSystem
	{
	public:
		ResourceManager() : mXMLFile(RESOURCE_INDEX_FILE_NAME) {};
		~ResourceManager() = default;

		void startUp() override;
		void shutDown() override {};
				
		/* retrieve the object that corresponds to the name passed as the argument
		if the object is not yet loaded in memory it loads it
		note that name is also the name of the resource file that the returned
		object manages
		*/
		template<typename T>
		std::shared_ptr<T> get(const std::string& name)
		{
			uint32_t key = SID(name.c_str());			
			const auto & iterator = mResources.find(key);
			if (iterator != mResources.end()) //is the resource registered?
			{						
				if (iterator->second->resourcePointer) //is the resource already loaded?
				{
					return std::dynamic_pointer_cast<T>(iterator->second->resourcePointer); //bad idea...
				}else
				{				
					XMLNode metaDataNode = iterator->second->metaData;					
					std::shared_ptr<ResourceBase> resource = iterator->second->load(metaDataNode);
					iterator->second->resourcePointer = resource;
					return std::dynamic_pointer_cast<T>(resource); //this is probably a bad idea :/
				}
			}
			else {
				//mResourcesRegistered++;
				//constructs the object in place
				//mUsedBytes += (sizeof(resource->first) + sizeof(static_cast<T&>(*(resource->second))));		
				LOG_WARNING("resource " << name << " is not registered");
				std::shared_ptr<T> rp = nullptr;
				return rp;
			}
		}
		
		template<typename T, typename... Params>
		void registerResource(const std::string& name, Params&&... parameters)
		{
			uint32_t key = SID(name.c_str());
			auto iterator = mResources.find(key);
			if (iterator != mResources.end()) //is the resource already loaded?
			{
				//return std::dynamic_pointer_cast<T>(iterator->second->resourcePointer);
			}
			else {
				mResourcesRegistered++;
				std::shared_ptr<ResourceData> resourceData = std::make_shared<ResourceData>();
				resourceData->resourcePointer = std::make_shared<T>(std::forward<Params>(parameters)...);
				mResources.emplace(std::make_pair(key, resourceData));
				//constructs the object in place
				//const auto& resource = mResources.emplace(std::make_pair(key, std::make_shared<T>(std::forward<Params>(parameters)...))).first;
				//mUsedBytes += (sizeof(resource->first) + sizeof(static_cast<T&>(*(resource->second))));
				//return std::dynamic_pointer_cast<T>(resourceData->resourcePointer);
			}
		}

		/**
		register a resource with the resource manager to be loaded
		at a later time, if the resource is already registered
		then it does nothing
		*/		
		void registerResource(const XMLNode &metaData)
		{			
			//std::string a = metaData.getAttributeValue("name");
			uint32_t key = SID(metaData.getAttributeValue("name").c_str());
			if (mResources.find(key) == mResources.end()) //is the resource not registered already?
			{				
				std::shared_ptr<ResourceData> resourceData = std::make_shared<ResourceData>();
				resourceData->metaData = metaData;
				const auto & iterator = mLoaders.find(metaData.getName());
				if (iterator != mLoaders.end()) //is the resource loader registered?
				{
					resourceData->load = iterator->second;
					mResources.emplace(std::make_pair(key, resourceData));
					mResourcesRegistered++;
				}
				else {
					LOG_WARNING("Can't load resource:" << metaData.getAttributeValue("name") <<
					" resource loader not registered for type " << metaData.getName());
				}
			}
			return;
		}

		void registerLoader(const std::string &name, std::function<std::shared_ptr<ResourceBase>(const XMLNode &)> loader);

	private:
		std::unordered_map<uint32_t, std::shared_ptr<ResourceData>> mResources;		
		size_t mUsedBytes = 0;
		size_t mResourcesRegistered = 0;
		std::unordered_map<std::string, std::function<std::shared_ptr<ResourceBase>(const XMLNode &)>> mLoaders;
		XMLFile mXMLFile;
	};	
}
