
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

#include "GPUProgramParameter.h"
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

namespace Nova
{
GPUProgramParameterMat4::GPUProgramParameterMat4(const GLuint location, const float *parameter)
    : mData(parameter), mLocation(location)
{
}
void GPUProgramParameterMat4::update() // warning: program has to be already binded!
{
    glUniformMatrix4fv(mLocation, 1, GL_FALSE, mData);
}

GPUProgramParameterVec3::GPUProgramParameterVec3(const GLuint location, const float *parameter)
    : mData(parameter), mLocation(location)
{
}
void GPUProgramParameterVec3::update() // warning: program has to be already binded!
{
    glUniform3fv(mLocation, 1, mData);
}

GPUProgramParameterVec4::GPUProgramParameterVec4(const GLuint location, const float *parameter)
    : mData(parameter), mLocation(location)
{
}
void GPUProgramParameterVec4::update() // warning: program has to be already binded!
{
    glUniform4fv(mLocation, 1, mData);
}
GPUProgramParameterFloat::GPUProgramParameterFloat(const GLuint location, const float parameter)
    : mData(parameter), mLocation(location)
{
}
void GPUProgramParameterFloat::update() { glUniform1f(mLocation, mData); }
GPUProgramParameterInt::GPUProgramParameterInt(const GLuint location, const int parameter)
    : mData(parameter), mLocation(location)
{
}
void GPUProgramParameterInt::update() { glUniform1i(mLocation, mData); }
} // namespace Nova
