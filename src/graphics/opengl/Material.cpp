
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

#include "Material.h"
#include "graphics/ITexture.h"
#include "graphics/opengl/GPUProgram.h"
#include "graphics/opengl/Texture.h"
#include "graphics/opengl/TextureCube.h"
#include <memory>
#include <vector>

namespace Nova
{
/*Material::Material(GPUProgram & program, std::vector<ITexture*> texArray, MaterialProperties &
properties) : mProgram(program), mTextureArray(texArray), mAmbientColor(properties.ambient),
        mDiffuseColor(properties.diffuse), mSpecularColor(properties.specular)
{
}*/
// Material::Material(const std::shared_ptr<GPUProgram> &program, const MaterialProperties
// &properties) 	:mProgram(program), mAmbient(properties.ambient),
// mDiffuse(properties.diffuse),
//	mSpecular(properties.specular)
//{
//}
// Material::Material(const std::shared_ptr<GPUProgram> &program, const std::shared_ptr<ITexture>
// &diffuse, 	const MaterialProperties &properties) 	:mProgram(program),
// mAmbient(properties.ambient),
//	mDiffuse(properties.diffuse), mSpecular(properties.specular)
//{
//	//if (diffuse.get() != nullptr) { // WTF?? !@#$%%^
//		mTextures.push_back(diffuse);
//		mSamplerNames.push_back("diffuse0");
//	/*}
//	else {
//		LOG_DEBUG("$E#$#@");
//	}*/
//}
// Material::Material(const std::shared_ptr<GPUProgram> &program, const std::shared_ptr<ITexture>
// &diffuse, 	const std::shared_ptr<ITexture> &specular) 	:mProgram(program)
//{
//	mTextures.push_back(diffuse);
//	mTextures.push_back(specular);
//	mSamplerNames.push_back("diffuse0");
//	mSamplerNames.push_back("specular0");
//}
Material::Material(const std::shared_ptr<GPUProgram> &program,
                   const std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> &textures)
    : mProgram(program)
{
    setTextures(textures);
}
void Material::setTextures(
    const std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> &textures)
{
    mTextures.clear();
    mSamplerNames.clear();
    for (auto &textureNamePair : textures)
    {
        mTextures.push_back(textureNamePair.first);
        mSamplerNames.push_back(textureNamePair.second);
    }
}
/*const Vec3& Material::getAmbientColor() const
{
        return mAmbient;
}
const Vec3& Material::getDiffuseColor() const
{
        return mDiffuse;
}
const Vec4& Material::getSpecularColor() const
{
        return mSpecular;
}*/
/*void Material::setDiffuseTexture(const std::shared_ptr<ITexture> &tex)
{
        mDiffuseTexture = tex;
}
void Material::setSpecularTexture(const std::shared_ptr<ITexture> &tex)
{
        mSpecularTexture = tex;
}*/
/*void Material::setAmbientColor(const Vec3 &color)
{
        mAmbient = color;
}
void Material::setDiffuseColor(const Vec3 &color)
{
        mDiffuse = color;
}
void Material::setSpecularColor(const Vec4 &color)
{
        mSpecular = color;
}*/
void Material::bind() const
{
    mProgram->bind();
    /*if (mDiffuseTexture)
    {
            glActiveTexture(GL_TEXTURE0);
            mDiffuseTexture->bind();
            glUniform1i(glGetUniformLocation(mProgram->getProgramID(), "diffuse0"), 0);
    }
    if (mSpecularTexture)
    {
            glActiveTexture(GL_TEXTURE1);
            mSpecularTexture->bind();
            glUniform1i(glGetUniformLocation(mProgram->getProgramID(), "specular0"), 1);
    }*/

    int index = 0;
    if (!mTextures.empty() && mTextures[0].get())
    {
        for (auto &texture : mTextures)
        {
            glActiveTexture(GL_TEXTURE0 + index);
            texture->bind();
            glUniform1i(
                glGetUniformLocation(mProgram->getProgramID(), mSamplerNames[index].c_str()),
                index);

            index++;
        }
    }
}
void Material::unBind() const
{
    mProgram->unBind();
    ///*for (ITexture* texture : mTextureArray)
    //{
    //	texture->unBind();
    //}*/
    // if (mDiffuseTexture)
    //{
    //	mDiffuseTexture->unBind();
    //}
    // if (mSpecularTexture)
    //{
    //	mSpecularTexture->unBind();
    //}
    for (auto &texture : mTextures)
    {
        texture->unBind();
    }
    glActiveTexture(GL_TEXTURE0);
}
} // namespace Nova