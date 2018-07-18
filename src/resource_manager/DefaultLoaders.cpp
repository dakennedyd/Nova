#include "DefaultLoaders.h"
#include "PreInit.h"
#ifdef NOVA_WINDOWS_PLATFORM
#    include "windows/FileSystem.h"
#else
#    include "linux/FileSystem.h"
#endif
#include "ModelLoader.h"
//#include "Init.h"
#include "Xml.h"
#include "graphics/ITexture.h"
#ifdef NOVA_OPENGL
#    include "graphics/opengl/GPUProgram.h"
#    include "graphics/opengl/Material.h"
#    include "graphics/opengl/Mesh.h"
#    include "graphics/opengl/RendererBackendDeferred.h"
#    include "graphics/opengl/Texture.h"
#    include "graphics/opengl/TextureCube.h"
#endif
#include "logger/Logger.h"
#include "math/miscellaneous.h"
#include "resource_manager/ResourceBase.h"
#include "resource_manager/ResourceManager.h"
#include <memory>
#include <string>
#include <vector>

namespace Nova
{
std::shared_ptr<ResourceBase> loadTexture(const XMLNode &metadata)
{
    std::string name{metadata.getAttributeValue("name")};
    std::string type{metadata.getAttributeValue("type")};
    std::string filtering{metadata.getAttributeValue("filtering")};
    std::string src{TEXTURES_PATH + metadata.getText()};
    if (type == "2d")
    {
        auto texPtr = std::make_shared<Texture>(src);
        if (filtering == "linear")
            texPtr->setFiltering(Filtering::LINEAR);
        else if (filtering == "nearest")
            texPtr->setFiltering(Filtering::NEAREST);
        else
        {
            LOG_WARNING("texture:" << name << " filtering type not recognized, assuming linear");
            texPtr->setFiltering(Filtering::LINEAR);
        }
        return texPtr;
    }
    else if (type == "cube")
    {
        return std::make_shared<TextureCube>(src);
    }
    else if (type == "IBL")
    {
        std::string irradianceTextureFilePath{"NO TEXTURE"};
        XMLNode irradianceNode{metadata.getChildElement("irradiance")};
        if (!irradianceNode.isEmpty())
        {
            irradianceTextureFilePath = irradianceNode.getText();
        }
        auto irradiance = std::make_shared<TextureCube>(TEXTURES_PATH + irradianceTextureFilePath);
        XMLNode radianceLOD{metadata.getChildElement("radiance_LOD")};
        std::vector<std::string> filenames;
        while (!radianceLOD.isEmpty())
        {
            filenames.push_back(TEXTURES_PATH + radianceLOD.getText());
            radianceLOD = radianceLOD.getNextElement();
        }
        TextureInfo t = FileSystem::getInstance().loadTexture(filenames, false);
        auto radiance = std::make_shared<TextureCube>(t);
        // auto ibl = std::make_shared<IBL_Data>(irradiance, radiance);
        // ibl->irradiance = irradiance;
        // ibl->radiance = radiance;
        return std::make_shared<IBL_Data>(irradiance, radiance);
    }
    else
    {
        LOG_WARNING("texture resource " << name << " has an unrecognized type");
        return std::make_shared<Texture>(100, 100);
    }
}

std::shared_ptr<ResourceBase> loadShader(const XMLNode &metadata)
{
    std::string name{metadata.getAttributeValue("name")};
    XMLNode programNode(metadata.getChildElement(RENDERER));
    std::string src{SHADERS_PATH + programNode.getText()};
    return std::make_shared<GPUProgram>(src);
}

/*this is just a placeholder loader until i implement pbr materials */
std::shared_ptr<ResourceBase> loadMaterial(const XMLNode &metadata)
{
    std::string name{metadata.getAttributeValue("name")};
    std::string program(metadata.getChildElement("program").getText());
    XMLNode texture{metadata.getChildElement("textures").getFirstChild()};
    /*XMLNode propertiesNode{metadata.getChildElement("properties")};
    auto ambient{ vectorStringToFloat(split(propertiesNode.getAttributeValue("ambient"))) };
    auto diffuse{ vectorStringToFloat(split(propertiesNode.getAttributeValue("diffuse"))) };
    auto specularColor{
    vectorStringToFloat(split(propertiesNode.getAttributeValue("specularColor"))) }; auto
    specularIntensity = std::stof(propertiesNode.getAttributeValue("specularIntensity"));
    MaterialProperties properties{ Vec3{ambient[0],ambient[1],ambient[2] }, Vec3{
    diffuse[0],diffuse[1],diffuse[2] }, Vec4{ specularColor[0],specularColor[1] ,specularColor[2],
    specularIntensity } };*/

    /*MaterialProperties yellowPlastic(Vec3(0.0f), Vec3(0.5f, 0.5f, 0.0f), Vec4(0.6f, 0.6f, 0.5f,
    0.25f)); MaterialProperties jade(Vec3(0.135f, 0.2225f, 0.1575f), Vec3(0.54f, 0.89f, 0.63f),
    Vec4(0.316228f, 0.316228f, 0.316228f, 0.1f)); MaterialProperties copper(Vec3(0.19125f, 0.0735f,
    0.0225f), Vec3(0.7038f, 0.27048f, 0.0828f), Vec4(0.256777f, 0.137622f, 0.086014f, 0.1f));
    MaterialProperties pearl(Vec3(0.25f, 0.20725f, 0.20725f), Vec3(1.0f, 0.829f, 0.829f),
    Vec4(0.296648f, 0.296648f, 0.296648f, 0.088f));
    MaterialProperties plain(Vec3(0.0f), Vec3(1.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0078125f));*/
    ResourceManager &rm = ResourceManager::getInstance();
    std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> MaterialParams;
    while (!texture.isEmpty())
    {
        // std::string type{ texture.getAttributeValue("type") };
        /*if (type == "diffuse")
        {
                std::string textureName{ texture.getText() };
                return std::make_shared<Material>(rm.get<GPUProgram>(program),
        rm.get<ITexture>(textureName), properties);
        }
        else if (type == "specular")
        {
        


        }*/
        // std::string textureName{ texture.getText() };
        // std::shared_ptr<ITexture> a = rm.get<ITexture>(textureName);
        std::pair<std::shared_ptr<ITexture>, std::string> pair; // = std::make_pair<a, type >;
        pair.first = rm.get<ITexture>(texture.getText());
        pair.second = texture.getAttributeValue("type");
        MaterialParams.push_back(pair);

        texture = texture.getNextElement();
    }
    return std::make_shared<Material>(rm.get<GPUProgram>(program), MaterialParams);
    // return std::make_shared<Material>(rm.get<GPUProgram>(program), properties);
    // return std::make_shared<ResourceBase>();
}

std::shared_ptr<ResourceBase> loadMesh(const XMLNode &metadata)
{
    LOG_DEBUG("loading mesh:" << metadata.getAttributeValue("name"));
    // std::string ds{metadata.getName()};
    std::string name{metadata.getAttributeValue("name")};
    std::string type{metadata.getAttributeValue("type")};
    XMLNode param(metadata.getChildElement("parameters"));

    if (type == "icosphere")
    {
        std::string radius{param.getAttributeValue("radius")};
        std::string iterations{param.getAttributeValue("iterations")};
        return std::make_shared<Mesh>(
            Mesh::makeIcoSphere(std::stof(radius), std::stoi(iterations)));
    }
    else if (type == "box")
    {
        std::string length{param.getAttributeValue("length")};
        std::string width{param.getAttributeValue("width")};
        std::string height{param.getAttributeValue("height")};
        std::string texCoords{param.getAttributeValue("texcoords")};
        return std::make_shared<Mesh>(Mesh::makeBox(std::stof(length), std::stof(width),
                                                    std::stof(height),
                                                    vectorStringToFloat(split(texCoords))));
    }
    else if (type == "quad")
    {
        std::string width{param.getAttributeValue("width")};
        std::string height{param.getAttributeValue("height")};
        std::string texCoords{param.getAttributeValue("texcoords")};
        return std::make_shared<Mesh>(Mesh::makeQuad(std::stof(width), std::stof(height),
                                                     vectorStringToFloat(split(texCoords))));
    }
    else if (type == "skybox")
    {
        return std::make_shared<Mesh>(Mesh::makeSkyBoxMesh());
    }
    else if (type == "file")
    {
        std::string src{MODELS_PATH + metadata.getChildElement("src").getText()};
        return loadModel(src);
    }
    else
    {
        LOG_WARNING("mesh resource " << name << " has an unrecognized type");
        return std::make_shared<Mesh>(Mesh::makeBox());
    }
}

} // namespace Nova
