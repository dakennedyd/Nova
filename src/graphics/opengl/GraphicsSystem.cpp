
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

#include "GraphicsSystem.h"
#include "Error.h"
#include "RendererInit.h"
#include "Settings.h"
#include "linux/Window.h"
#include "logger/Logger.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Nova
{
// RendererBackend GraphicsSystem::mRenderer;
void GraphicsSystem::startUp()
{
    LOG_INFO("Initiating graphics subsystem");
    // mCurrentCamera = new FPSCamera();
    //================================================================
    // OPENGL initialization
    //================================================================
    // glfwSetKeyCallback(mWindow, keyCallback); //put keyboard handlers on keyboard.cpp
    // glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // start GLEW extension handler
    // glewExperimental = GL_TRUE;
    // glewInit();
    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    if (!gladLoadGL())
    {
        error("could not start GLAD");
    }
    // get version info
    const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte *version = glGetString(GL_VERSION);   // version as a string
    const GLubyte *vendor = glGetString(GL_VENDOR);
    LOG_INFO("RendererBackend:" << renderer);
    LOG_INFO("OpenGL version:" << version << " Vendor:" << vendor);
    checkExtensionsSupport();
    //glEnable(GL_MULTISAMPLE);
    int width, height;
    glfwGetFramebufferSize(Window::getInstance().mGLFWindow, &width, &height);
    glViewport(0, 0, width, height); // create a a viewport as big as the framebuffer
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    // glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // cull back face
    glFrontFace(GL_CW);  // GL_CCW for counter clock-wise
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    // glEnable(GL_STENCIL_TEST);
    // glStencilFunc(GL_EQUAL, 1, 0xFF);//discard fragments where stencil is not equal to 1
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //set rasterizer to only draw lines(wireframe)
    /*glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

    // glViewport(0, 0, Window::getInstance().getWidth(), Window::getInstance().getHeight());

#ifndef NDEBUG // if debug build
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback((GLDEBUGPROC)openglErrorCallback, 0);
#else
    glDisable(GL_DEBUG_OUTPUT);
#endif
}

void GraphicsSystem::shutDown() {}

void GraphicsSystem::checkExtensionsSupport()
{
    GLint num = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num);
    for (GLint i = 0; i < num; i++)
    {
        // const char *e = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
        std::string extention(reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, i)));
        // int strSize = 0;
        // while(e[strSize] != NULL)
        // strSize++;
        // std::string ext(*e, strSize);
        // ext.erase(std::remove(ext.begin(), ext.end(), ' '), ext.end()); //remove white space
        if (extention == "GL_EXT_texture_filter_anisotropic")
        // if (strcmp(e, "GL_EXT_texture_filter_anisotropic") == 0)
        {
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &mMaxAnisotropyLevel);
            // LOG_INFO((const char*)e << " " << mMaxAnisotropyLevel);
        }
    }
    LOG_INFO("Max anisotropy level:" << mMaxAnisotropyLevel);
}
GLfloat GraphicsSystem::getMaxAnisotropy() { return mMaxAnisotropyLevel; }
void GraphicsSystem::setWireframeMode(bool state)
{
    if (state == true)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // set rasterizer to only draw lines(wireframe)
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
/*RendererBackend & GraphicsSystem::getRendererBackend()
{
        return *mRenderer;
}*/
} // namespace Nova