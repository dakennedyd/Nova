
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

#include "GPUProgram.h"
#include "Error.h"
#include "graphics/opengl/RendererInit.h"
#include "logger/Logger.h"
#ifdef NOVA_WINDOWS_PLATFORM
#    include "windows/FileSystem.h"
#else
#    include "linux/FileSystem.h"
#endif
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iterator>
#include <string>
#include <vector>

namespace Nova
{
GPUProgram::GPUProgram(const std::string &fileAndPath) 
: mProgramID(0), mPathAndFile(fileAndPath)
{
    LOG_DEBUG("Loading shader:" << mPathAndFile);
    recompile();
}

GPUProgram::~GPUProgram() { glDeleteProgram(mProgramID); }

void GPUProgram::recompile(std::vector<std::pair<enum ShaderType, std::string>> defines)
{
    glDeleteProgram(mProgramID);    
    mProgramID = glCreateProgram();
    size_t secondLinePosition = 0;
    std::string srcFile(FileSystem::getInstance().loadFileAsString(mPathAndFile));

    if (srcFile.find_first_of(std::string("#version")) != std::string::npos)
    {
        secondLinePosition = srcFile.find_first_of(std::string("\n"));
        if (secondLinePosition == std::string::npos)
        { 
            LOG_ERROR("Malformed shader source file");
            glDeleteProgram(mProgramID);
            return;
        }
    }
    else{
        LOG_ERROR("Malformed shader source file");
        glDeleteProgram(mProgramID);
        return;
    }
    secondLinePosition++; // ugly
    size_t linePositionVS = secondLinePosition;
    size_t linePositionFS = secondLinePosition;
    std::string vs(srcFile);
    std::string ps(srcFile);
    std::string vsDefine = "#define NOVA_VERTEX_SHADER\n";
    std::string fsDefine = "#define NOVA_FRAGMENT_SHADER\n";
    vs.insert(secondLinePosition, vsDefine);
    linePositionVS+=vsDefine.size();
    ps.insert(secondLinePosition, fsDefine);
    linePositionFS+=fsDefine.size();
    
    for(auto &define : defines)
    {
        if(define.first == ShaderType::VERTEX_SHADER){
            vs.insert(linePositionVS, define.second + "\n");
            linePositionVS+=define.second.size();
        }else if(define.first == ShaderType::FRAGMENT_SHADER)
        {
            ps.insert(linePositionFS, define.second + "\n");
            linePositionFS+=define.second.size();
        }
    }
    construct(vs, ps, mPathAndFile);
}

void GPUProgram::bind() const { glUseProgram(mProgramID); }
void GPUProgram::unBind() const { glUseProgram(0); }

void GPUProgram::construct(const std::string &vertexShader, const std::string &pixelShader,
                           const std::string &filename)
{
    // ======================== Vertex Shader ========================

    GLuint vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *glVertexShaderSrc = vertexShader.c_str();
    glShaderSource(vertexShaderHandle, 1, &glVertexShaderSrc, NULL);
    glCompileShader(vertexShaderHandle);
    // check for compile errors
    int params = -1;
    glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params)
    {
        GLint logLength;
        glGetShaderiv(vertexShaderHandle, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> v(logLength);
        glGetShaderInfoLog(vertexShaderHandle, logLength, NULL, v.data());
        std::string str(std::begin(v), std::end(v) - 2);
        LOG_ERROR("GL vertex shader " + filename + " index " + std::to_string(vertexShaderHandle) +
                  " did not compile");
        LOG_ERROR(str);
        //error(str);
    }

    // ======================= Fragment Shader =======================

    GLuint fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *glFragmentShaderSrc = pixelShader.c_str();
    glShaderSource(fragmentShaderHandle, 1, &glFragmentShaderSrc, NULL);
    glCompileShader(fragmentShaderHandle);
    // check for compile errors
    params = -1;
    glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params)
    {
        GLint logLength;
        glGetShaderiv(fragmentShaderHandle, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> v(logLength);
        glGetShaderInfoLog(fragmentShaderHandle, logLength, NULL, v.data());
        std::string str(std::begin(v), std::end(v) - 2);
        LOG_ERROR("GL fragment shader " + filename + " index " +
                  std::to_string(fragmentShaderHandle) + " did not compile");
        LOG_ERROR(str);
        //error(str);
    }

    // =========================== Program ===========================

    // mProgramID = glCreateProgram();
    glAttachShader(mProgramID, fragmentShaderHandle);
    glAttachShader(mProgramID, vertexShaderHandle);

    /*glBindAttribLocation(handle, 0, "vertex_position");		//attrib location must
    always be set on the shader glBindAttribLocation(handle, 1, "vertex_colour");*/
    glLinkProgram(mProgramID);

    // check if link was successful
    params = -1;
    glGetProgramiv(mProgramID, GL_LINK_STATUS, &params);
    if (GL_TRUE != params)
    {
        LOG_ERROR("could not link shader " + filename + " program GL index " +
              std::to_string(mProgramID));
    }

    // we dont need those anymore
    glDetachShader(mProgramID, vertexShaderHandle);
    glDeleteShader(vertexShaderHandle);
    glDetachShader(mProgramID, fragmentShaderHandle);
    glDeleteShader(fragmentShaderHandle);
}


} // namespace Nova