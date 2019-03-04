
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

void Material::bind() const
{
    mProgram->bind();
    int index = 0;
    //if (!mTextures.empty() /*&& mTextures[0].get()*/)
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
    //int index = 0;
    for (auto &texture : mTextures)
    {
        // glActiveTexture(GL_TEXTURE0 + index);
        // glUniform1i(
        //         glGetUniformLocation(mProgram->getProgramID(), mSamplerNames[index].c_str()),
        //         0);
        // index++;
        texture->unBind();
    }
    glActiveTexture(GL_TEXTURE0);
}
} // namespace Nova