
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
