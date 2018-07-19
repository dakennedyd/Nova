
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
#include "graphics/IMaterial.h"
#include "graphics/ITexture.h"
#include "graphics/opengl/GPUProgram.h"
#include <memory>
#include <vector>

namespace Nova
{
/*struct MaterialProperties
{
        MaterialProperties(const Vec3& amb, const Vec3& diff, const Vec4& spec)
                :ambient(amb), diffuse(diff), specular(spec) {}
        Vec3 ambient;
        Vec3 diffuse;
        Vec4 specular;
};*/

class Material : public IMaterial
{
  public:
    Material() = delete;
    // Material(GPUProgram &program, std::vector<ITexture*> texArray, MaterialProperties &
    // properties);
    /*Material(const std::shared_ptr<GPUProgram> &program, const MaterialProperties &properties =
    MaterialProperties(Vec3{ 0.0f }, Vec3{ 1.0f }, Vec4{0.0f, 0.0f, 0.0f, 0.0078125f }));
    Material(const std::shared_ptr<GPUProgram> &program, const std::shared_ptr<ITexture> &diffuse,
    const MaterialProperties &properties = MaterialProperties(Vec3{ 0.0f }, Vec3{ 1.0f }, Vec4{
    0.0f, 0.0f, 0.0f, 0.0078125f })); Material(const std::shared_ptr<GPUProgram> &program, const
    std::shared_ptr<ITexture> &diffuse, const std::shared_ptr<ITexture> &specular);*/
    Material(const std::shared_ptr<GPUProgram> &program,
             const std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> &textures);
    ~Material() = default;
    const std::shared_ptr<GPUProgram> &getGPUProgram() const { return mProgram; };
    void setProgram(const std::shared_ptr<GPUProgram> &program) { mProgram = program; };
    void
    setTextures(const std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> &textures);
    /*const Vec3& getAmbientColor() const;
    const Vec3& getDiffuseColor() const;
    const Vec4& getSpecularColor() const;*/
    /*void setDiffuseTexture(const std::shared_ptr<ITexture> &tex);
    void setSpecularTexture(const std::shared_ptr<ITexture> &tex);*/
    /*void setAmbientColor(const Vec3 &color);
    void setDiffuseColor(const Vec3 &color);
    void setSpecularColor(const Vec4 &color);*/

    void bind() const override;
    void unBind() const override;

  private:
    std::vector<std::shared_ptr<ITexture>> mTextures; // texture can be a Texture or CubeTexture
    std::vector<std::string>
        mSamplerNames; // corresponding alphanumeric names for the texture samplers in GLSL code
    std::shared_ptr<GPUProgram> mProgram = nullptr;
    /*std::shared_ptr<ITexture> mDiffuseTexture = nullptr;
    std::shared_ptr<ITexture> mSpecularTexture = nullptr;*/
    // GPUProgram *mProgram;
    // ITexture* mDiffuseTexture = nullptr;
    // ITexture* mSpecularTexture = nullptr;
    /*Vec3 mAmbient;
    Vec3 mDiffuse;
    Vec4 mSpecular;*/
};
} // namespace Nova
