
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

#pragma once
#include "Crc.h"
#include "ISingleton.h"
#include "ISubSystem.h"
#include "ResourceBase.h"
#include "Xml.h"
#include "logger/Logger.h"
#include <memory>
#include <unordered_map>

namespace Nova
{
struct ResourceData
{
    std::shared_ptr<ResourceBase> resourcePointer;
    // bool isLoaded = false;
    XMLNode metaData;
    // std::shared_ptr<ResourceBase>(*load)(const XMLNode &);
    std::function<std::shared_ptr<ResourceBase>(const XMLNode &)> load;
};

class ResourceManager : public ISingleton<ResourceManager>, public ISubSystem
{
  public:
    ResourceManager()
        //: mEngineResourceIndex(PATH_TO_ENGINE_BINARY + RESOURCE_INDEX_FILE_NAME){};
        : mApplicationResourceIndex(PATH_TO_BINARY + RESOURCE_INDEX_FILE_NAME){};
    ~ResourceManager() = default;

    void startUp() override;
    void shutDown() override{};

    /**
     * @brief retrieve the object that corresponds to the name passed as the argument
     if the object is not yet loaded in memory it loads it
     note that name is also the name of the resource file that the returned
     object manages
     *
     * @tparam T
     * @param name
     * @return std::shared_ptr<T>
     */
    template <typename T> std::shared_ptr<T> get(const std::string &name)
    {
        uint32_t key = SID(name.c_str());
        const auto &iterator = mResources.find(key);
        if (iterator != mResources.end()) // is the resource registered?
        {
            if (iterator->second->resourcePointer) // is the resource already loaded?
            {
                return std::dynamic_pointer_cast<T>(
                    iterator->second->resourcePointer); // bad idea...
            }
            else
            {
                XMLNode &metaDataNode = iterator->second->metaData;
                // std::shared_ptr<ResourceBase> resource = iterator->second->load(metaDataNode);
                iterator->second->resourcePointer = iterator->second->load(metaDataNode);
                return std::dynamic_pointer_cast<T>(
                    iterator->second->resourcePointer); // this is probably a bad idea :/
            }
        }
        else
        {
            // mResourcesRegistered++;
            // constructs the object in place
            // mUsedBytes += (sizeof(resource->first) +
            // sizeof(static_cast<T&>(*(resource->second))));
            LOG_WARNING("resource " << name << " is not registered");
            std::shared_ptr<T> rp = nullptr;
            return rp;
        }
    }

    // template <typename T, typename... Params>
    // void registerResource(const std::string &name, Params &&... parameters)
    // {
    //     uint32_t key = SID(name.c_str());
    //     auto iterator = mResources.find(key);
    //     if (iterator != mResources.end()) // is the resource already registered?
    //     {
    //         // return std::dynamic_pointer_cast<T>(iterator->second->resourcePointer);
    //         LOG_WARNING("Trying to register already loaded resource "
    //                     << iterator->second->metaData.getName());
    //     }
    //     else
    //     {
    //         mResourcesRegistered++;
    //         std::shared_ptr<ResourceData> resourceData = std::make_shared<ResourceData>();
    //         resourceData->resourcePointer =
    //             std::make_shared<T>(std::forward<Params>(parameters)...);
    //         mResources.emplace(std::make_pair(key, resourceData));
    //         // constructs the object in place
    //         // const auto& resource = mResources.emplace(std::make_pair(key,
    //         // std::make_shared<T>(std::forward<Params>(parameters)...))).first; mUsedBytes +=
    //         // (sizeof(resource->first) + sizeof(static_cast<T&>(*(resource->second)))); return
    //         // std::dynamic_pointer_cast<T>(resourceData->resourcePointer);
    //     }
    // }

    /**
    register a resource with the resource manager to be loaded
    at a later time, if the resource is already registered
    then it does nothing
    */
    void registerResource(const XMLNode &metaData)
    {
        // std::string a = metaData.getAttributeValue("name");
        uint32_t key = SID(metaData.getAttributeValue("name").c_str());
        if (mResources.find(key) == mResources.end()) // is the resource not registered already?
        {
            std::shared_ptr<ResourceData> resourceData = std::make_shared<ResourceData>();
            resourceData->metaData = metaData;
            const auto &iterator = mLoaders.find(metaData.getName());
            if (iterator != mLoaders.end()) // is the resource loader registered?
            {
                resourceData->load = iterator->second;
                mResources.emplace(std::make_pair(key, resourceData));
                mResourcesRegistered++;
            }
            else
            {
                LOG_WARNING("Can't load resource:" << metaData.getAttributeValue("name")
                                                   << " resource loader not registered for type "
                                                   << metaData.getName());
            }
        }
        else
        {
            LOG_WARNING("resource:" << metaData.getAttributeValue("name") << " already registered");
        }
        return;
    }

    void registerLoader(const std::string &name,
                        std::function<std::shared_ptr<ResourceBase>(const XMLNode &)> loader);

  private:
    std::unordered_map<uint32_t, std::shared_ptr<ResourceData>> mResources;
    size_t mUsedBytes = 0;
    size_t mResourcesRegistered = 0;
    std::unordered_map<std::string, std::function<std::shared_ptr<ResourceBase>(const XMLNode &)>>
        mLoaders;
    //XMLFile mEngineResourceIndex;
    XMLFile mApplicationResourceIndex;
};
} // namespace Nova
