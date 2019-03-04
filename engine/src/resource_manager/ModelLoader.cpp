
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

#include "ModelLoader.h"
#include "PreInit.h"
#ifdef NOVA_OPENGL
#    include "graphics/opengl/Mesh.h"
#endif
#ifdef NOVA_WINDOWS_PLATFORM
#    include "windows/FileSystem.h"
#elif defined NOVA_LINUX_PLATFORM
#    include "linux/FileSystem.h"
#endif
#include "Error.h"
#include "ResourceManager.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <memory>

namespace Nova
{
std::shared_ptr<Mesh> loadedMesh = nullptr;

void processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    std::vector<float> normals;
    std::vector<float> texturesCoodinates;
    // Texture* diffuse;
    // Material* basicMaterial;
    // FileSystem& fs = FileSystem::getInstance();
    // auto &rm = ResourceManager::getInstance();
    // GPUProgram *program;
    // Mesh* novaMesh;

    if (mesh->mNumVertices < 1) LOG_WARNING("Mesh has no vertices!");
    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        // Process vertex positions, normals and texture coordinates
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        if (mesh->HasNormals())
        {
            normals.push_back(mesh->mNormals[i].x);
            normals.push_back(mesh->mNormals[i].y);
            normals.push_back(mesh->mNormals[i].z);
        }
        else
        {
            LOG_WARNING("Mesh doesn't have normals!");
        }

        if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
        {
            // only uses the first one for the moment
            texturesCoodinates.push_back(mesh->mTextureCoords[0][i].x);
            texturesCoodinates.push_back(mesh->mTextureCoords[0][i].y);
        }
        else
        {
            LOG_WARNING("Mesh has no texture coords!");
        }
    }
    // Process indices
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // basicMaterial = &(rm.get<Material>(mResourceName));

    // Process textures
    /*if (mesh->mMaterialIndex >= 0)
    {
            aiMaterial* assimpMaterial = scene->mMaterials[mesh->mMaterialIndex];
            if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                    aiString str;
                    assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str);
                    std::string textureFilename(str.C_Str());

                    //diffuse = new Texture((fs.loadTexture("../Resources/textures/" + a)));
                    //diffuse = &(rm.get<Texture>(TEXTURES_PATH + textureFilename));

                    //basicMaterial = new Material(*programWithTextures, *diffuse, plain);
                    //program = &(rm.get<GPUProgram>(SHADERS_PATH + "basicWithTextures.glsl"));
                    //basicMaterial->setDiffuseTexture(*diffuse);
            }
            else {
                    //program = &(rm.get<GPUProgram>(SHADERS_PATH + "basicNoTextures.glsl"));
            }
    }
    else {
            //program = &(rm.get<GPUProgram>(SHADERS_PATH + "basicNoTextures.glsl"));
    }*/
    // basicMaterial->setProgram(program);
    // MaterialProperties plain(Vec3(0.0f), Vec3(1.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0078125f));

    if (mesh->HasNormals())
    {
        if (mesh->mTextureCoords[0]) // does mesh has texture coordinates
        {
            // mMeshes.push_back(new Mesh(vertices, indices, normals, texturesCoodinates));
            // novaMesh = rmMesh.get()
            loadedMesh = std::make_shared<Mesh>(vertices, indices, normals, texturesCoodinates);
        }
        else
        {
            // mMeshes.push_back(new Mesh(vertices, indices, normals));
            loadedMesh = std::make_shared<Mesh>(vertices, indices, normals);
        }
    }
    else
    {
        // mMeshes.push_back(new Mesh(vertices, indices));
        loadedMesh = std::make_shared<Mesh>(vertices, indices);
    }
    // assert(!mMeshes.empty());

    // RendererBackend::getInstance().addPacket(new RenderPacket(*mMeshes.back(), *basicMaterial));
}

void processNode(aiNode *node, const aiScene *scene)
{
    // Process all the node's meshes (if any)
    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *assimpMesh = scene->mMeshes[node->mMeshes[i]];

        processMesh(assimpMesh, scene);
    }
    // Then do the same for each of its children
    for (GLuint i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

std::shared_ptr<ResourceBase> loadModel(const std::string &fileAndPath)
//:mDirectory(fileAndPath.substr(0, fileAndPath.find_last_of('/')))
{
    LOG_DEBUG("loading mesh file:" << fileAndPath);
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(
        fileAndPath, aiProcess_OptimizeMeshes | aiProcess_Triangulate | aiProcess_FlipUVs /*|*/
        /* aiProcess_FlipWindingOrder */ /*|*/ /*aiProcess_MakeLeftHanded*/);

    if (scene && scene->mFlags != AI_SCENE_FLAGS_INCOMPLETE && scene->mRootNode)
    {
        // FileSystem& fs = FileSystem::getInstance();
        // const aiScene* scene = fs.loadModel(fileAndPath);

        processNode(scene->mRootNode, scene);
        return loadedMesh;
    }
    else
    {
        error("::ASSIMP::" + std::string{import.GetErrorString()});
        return loadedMesh; // code should never get here
    }
}

} // namespace Nova