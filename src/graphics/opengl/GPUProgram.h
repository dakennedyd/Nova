
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
#include "graphics/IGPUProgram.h"
#include "graphics/IGPUProgramParameter.h"
#include <glad/glad.h>

namespace Nova
{

class GPUProgram : public IGPUProgram
{
  public:
    // GPUProgram(const std::string &vertexShader, const std::string &pixelShader);
    GPUProgram(const std::string &fileAndPath);
    ~GPUProgram();

    void bind() const override;
    void unBind() const override;
    GLuint getProgramID() const { return mProgramID; }

    void recompile(std::vector<std::pair<enum ShaderType, std::string>> defines = {}) override;

  private:
    GLuint mProgramID = 0;
    std::string mPathAndFile; // not technically necessary but good to have

    void construct(const std::string &vertexShader, const std::string &pixelShader,
                   const std::string &filename);
};
} // namespace Nova
