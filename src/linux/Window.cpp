
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

#include "linux/Window.h"
#include "Error.h"
#include "Init.h"
#include "Settings.h"
#include <GLFW/glfw3.h>
//#include <glad/glad.h>

namespace Nova
{
void window_close_callback(GLFWwindow *window) { glfwSetWindowShouldClose(window, GLFW_TRUE); }

void Window::startUp()
{
    EngineSettings &es = EngineSettings::getInstance();
    // GLFW requires this to be set before window creation
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifndef NDEBUG // if debug build
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    if (!glfwInit())
    {
        error("could not start GLFW3");
    }
    // this hints have to be set after glfwInit for some reason
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWmonitor *mon = glfwGetPrimaryMonitor();
    const GLFWvidmode *vmode = glfwGetVideoMode(mon);

    // glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    if (es.getBoolean("Video", "fullscreen"))
    {
        mWidth = vmode->width;
        mHeight = vmode->height;
        mGLFWindow = glfwCreateWindow(mWidth, mHeight, "Nova", mon, nullptr);
    }
    else
    {
        mWidth = es.getInteger("Video", "width");
        mHeight = es.getInteger("Video", "height");
        mGLFWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
    }
    if (!mGLFWindow)
    {
        glfwTerminate();
        error("could not open window with GLFW3");
    }

    setTitle(NOVA_DESCRIPTION_STRING);
    // hideCursor();
    glfwSetWindowCloseCallback(mGLFWindow, window_close_callback);
    glfwSetWindowPos(mGLFWindow, 50, 50);
    // glfwSetCursorPosCallback(mGLFWindow, handleMouse);
    glfwSwapInterval(0);
    glfwMakeContextCurrent(mGLFWindow);
}

void Window::shutDown()
{
    glfwDestroyWindow(mGLFWindow); // not necesarily needed, glfwTerminate does the same
    // but good to have in case i implement multiple windows
    glfwTerminate();
}

int Window::getWidth() { return mWidth; }

int Window::getHeight() { return mHeight; }

bool Window::isClosing()
{
    if (glfwWindowShouldClose(mGLFWindow) == 1)
    {
        return true;
    }
    return false;
}

void Window::setTitle(const std::string &title)
{
    // glfwSetWindowTitle(mGLFWindow, std::string("NOVA | FPS:" +
    // std::to_string(static_cast<int>(1.0f / (mTimer.getMillis() / 1000.0f)))).c_str());
    glfwSetWindowTitle(mGLFWindow, title.c_str());
}
} // namespace Nova
