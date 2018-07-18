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
/*GPUProgram::GPUProgram(const std::string & vertexShader, const std::string & pixelShader)
{
        construct(vertexShader, pixelShader);
}*/

GPUProgram::GPUProgram(const std::string &fileAndPath) : mProgramID(glCreateProgram())
{
    size_t secondLinePosition = 0;
    std::string srcFile(FileSystem::getInstance().loadFileAsString(fileAndPath));

    if (srcFile.find_first_of(std::string("#version")) != std::string::npos)
    {
        secondLinePosition = srcFile.find_first_of(std::string("\n"));
        if (secondLinePosition == std::string::npos) error("Malformed shader source file");
    }
    else
        error("Malformed shader source file");
    secondLinePosition++; // ugly
    std::string vs(srcFile);
    vs.insert(secondLinePosition, "#define NOVA_VERTEX_SHADER\n");
    std::string ps(srcFile);
    ps.insert(secondLinePosition, "#define NOVA_FRAGMENT_SHADER\n");
    ps.insert(secondLinePosition + 29,
              "#define MAX_LIGHTS " + std::to_string(MAX_LIGHTS) + "\n"); // this is ugly!!!
    // ps = std::to_string(MAX_LIGHTS) + ps;
    construct(vs, ps, fileAndPath);
}

GPUProgram::~GPUProgram() { glDeleteProgram(mProgramID); }
/*unsigned int GPUProgram::getProgramID()
{
        return mProgramID;
}*/
void GPUProgram::bind() const { glUseProgram(mProgramID); }
void GPUProgram::unBind() const { glUseProgram(0); }

/*void GPUProgram::updateAllUniforms()
{
        for (IGPUProgramParameter* parameter : mParameters)
        {
                parameter->update();
        }
}

void GPUProgram::addParameter(IGPUProgramParameter * parameter)
{
        mParameters.push_back(parameter);
}*/

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
        error(str);
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
        error(str);
    }

    // =========================== Program ===========================

    // mProgramID = glCreateProgram();
    glAttachShader(mProgramID, fragmentShaderHandle);
    glAttachShader(mProgramID, vertexShaderHandle);

    /*glBindAttribLocation(handle, 0, "vertex_position");		//attrib location must always be set
    on the shader glBindAttribLocation(handle, 1, "vertex_colour");*/
    glLinkProgram(mProgramID);

    // check if link was successful
    params = -1;
    glGetProgramiv(mProgramID, GL_LINK_STATUS, &params);
    if (GL_TRUE != params)
    {
        error("could not link shader " + filename + " program GL index " +
              std::to_string(mProgramID));
    }

    // we dont need those anymore
    glDeleteShader(vertexShaderHandle);
    glDeleteShader(fragmentShaderHandle);
}

/*void GPUProgram::updateUniforms(int location)
{
        GLint count;

        GLint size; // size of the variable
        GLenum type; // type of the variable (float, vec3 or mat4, etc)

        const GLsizei bufSize = 16; // maximum name length
        GLchar name[bufSize]; // variable name in GLSL
        GLsizei length; // name length

        glGetProgramiv(mProgramID, GL_ACTIVE_UNIFORMS, &count);

        for (int i = 0; i < count; i++)
        {
                glGetActiveUniform(mProgramID, (GLuint)i, bufSize, &length, &size, &type, name);

                printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
        }
}*/
} // namespace Nova