#pragma once
#include <memory>
namespace Nova
{
class ResourceBase;
class XMLNode;
std::shared_ptr<ResourceBase> loadTexture(const XMLNode &metadata);
std::shared_ptr<ResourceBase> loadShader(const XMLNode &metadata);
std::shared_ptr<ResourceBase> loadMaterial(const XMLNode &metadata);
std::shared_ptr<ResourceBase> loadMesh(const XMLNode &metadata);
} // namespace Nova