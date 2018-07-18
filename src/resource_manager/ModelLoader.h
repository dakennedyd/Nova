#pragma once
#include "resource_manager/ResourceBase.h"
#include <memory>
#include <string>

namespace Nova
{
// temp class until i create some sort of entity component system
/*class Model :public ResourceBase
{
public:
        Model() = delete;
        Model(const std::string& fileAndPath);
        ~Model();

        const std::string& getResourceName() { return mResourceName; };
        void setResourceName(const std::string& name) { mResourceName = name; };
private:
        void processNode(aiNode* node, const aiScene* scene);
        void processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Mesh*> mMeshes; //this has to be a tree structure
        //std::string mDirectory;

        Mat4 mModel; //model matrix
        std::string mResourceName;
};*/
std::shared_ptr<ResourceBase> loadModel(const std::string &fileAndPath);
} // namespace Nova
