
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
#include "graphics/IGPUProgramParameter.h"
#include <glad/glad.h>

namespace Nova
{
class GPUProgramParameterMat4 : public IGPUProgramParameter
{
  public:
    GPUProgramParameterMat4() = delete;
    GPUProgramParameterMat4(const GLuint location, const float *parameter);
    ~GPUProgramParameterMat4() = default;

    void update() override;

  private:
    const float *mData;
    const GLuint mLocation;
};

class GPUProgramParameterVec3 : public IGPUProgramParameter
{
  public:
    GPUProgramParameterVec3() = delete;
    GPUProgramParameterVec3(const GLuint location, const float *parameter);
    // GPUProgramParameterVec3::GPUProgramParameterVec3(const GLuint location, const float x, const
    // float y, const float z);
    ~GPUProgramParameterVec3() = default;

    void update() override;

  private:
    const float *mData;
    // const float mData[3];
    const GLuint mLocation;
};

class GPUProgramParameterVec4 : public IGPUProgramParameter
{
  public:
    GPUProgramParameterVec4() = delete;
    GPUProgramParameterVec4(const GLuint location, const float *parameter);
    ~GPUProgramParameterVec4() = default;

    void update() override;

  private:
    const float *mData;
    const GLuint mLocation;
};

class GPUProgramParameterFloat : public IGPUProgramParameter
{
  public:
    GPUProgramParameterFloat() = delete;
    GPUProgramParameterFloat(const GLuint location, const float parameter);
    ~GPUProgramParameterFloat() = default;

    void update() override;

  private:
    const float mData;
    const GLuint mLocation;
};

class GPUProgramParameterInt : public IGPUProgramParameter
{
  public:
    GPUProgramParameterInt() = delete;
    GPUProgramParameterInt(const GLuint location, const int parameter);
    ~GPUProgramParameterInt() = default;

    void update() override;

  private:
    const int mData;
    const GLuint mLocation;
};
} // namespace Nova
