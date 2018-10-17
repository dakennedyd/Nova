
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

#include "ResourceManager.h"
#include "Timer.h"
#include "Xml.h"
#include "logger/Logger.h"
#include "resource_manager/DefaultLoaders.h"
#include "resource_manager/ResourceBase.h"
#include <functional>
#include <memory>

namespace Nova
{

void ResourceManager::startUp()
{
    Timer timer;
    LOG_INFO("Initiating the Resource Manager");

    /* registers loader functions for each resource supported */
    std::function<std::shared_ptr<ResourceBase>(const XMLNode &)> loadMaterialFunction =
        loadMaterial;
    registerLoader("material", loadMaterialFunction);
    std::function<std::shared_ptr<ResourceBase>(const XMLNode &)> loadMeshFunction = loadMesh;
    registerLoader("mesh", loadMeshFunction);
    std::function<std::shared_ptr<ResourceBase>(const XMLNode &)> loadTextureFunction = loadTexture;
    registerLoader("texture", loadTextureFunction);
    std::function<std::shared_ptr<ResourceBase>(const XMLNode &)> loadShaderFunction = loadShader;
    registerLoader("program", loadShaderFunction);
    std::function<std::shared_ptr<ResourceBase>(const XMLNode &)> loadSoundFunction = loadSound;
    registerLoader("sound", loadSoundFunction);

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
    LOG_INFO("Resource Manager initialized:" << mResourcesRegistered << " resources registered in "
                                             << timer.getMillis() << "ms.");
}
void ResourceManager::registerLoader(
    const std::string &name, std::function<std::shared_ptr<ResourceBase>(const XMLNode &)> loader)
{
    // mLoaders[name] = loader;
    mLoaders.emplace(std::make_pair(name, loader));
}
} // namespace Nova