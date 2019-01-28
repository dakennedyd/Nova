
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

class Material : public IMaterial
{
  public:
    Material() = delete;    
    Material(const std::shared_ptr<GPUProgram> &program,
             const std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> &textures = {});
    ~Material() = default;
    const std::shared_ptr<GPUProgram> &getGPUProgram() const { return mProgram; };
    void setProgram(const std::shared_ptr<GPUProgram> &program) { mProgram = program; };
    void
    setTextures(const std::vector<std::pair<std::shared_ptr<ITexture>, std::string>> &textures);
    
    void bind() const override;
    void unBind() const override;

  private:
    std::vector<std::shared_ptr<ITexture>> mTextures; // texture can be a Texture or CubeTexture
    std::vector<std::string>
        mSamplerNames; // corresponding alphanumeric names for the texture samplers in GLSL code
    std::shared_ptr<GPUProgram> mProgram = nullptr;    
};
} // namespace Nova
