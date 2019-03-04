
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
//#include <GL/glew.h> // GL extension handler library
//#include <glad/glad.h>
#include "ISingleton.h"
#include "ISubSystem.h"
#include "IWindow.h"
#include "Timer.h"
#include <GLFW/glfw3.h>

namespace Nova
{
class Window final : public IWindow, public ISingleton<Window>, public ISubSystem
{
  public:
    Window() = default;
    ~Window() = default;
    void startUp() override;
    void shutDown() override;
    int getWidth() override;
    int getHeight() override;
    bool isClosing() override;
    const std::string &getTitle() override { return mTitle; };
    void setTitle(const std::string &title) override;
    void swapFrameBuffers() override { glfwSwapBuffers(mGLFWindow); };
    void processEvents() override { glfwPollEvents(); };
    void minimize() override { glfwIconifyWindow(mGLFWindow); };
    void restore() override { glfwRestoreWindow(mGLFWindow); };
    void show() override { glfwShowWindow(mGLFWindow); };
    void showCursor() override
    {
        glfwSetInputMode(mGLFWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mCursorEnabled = true;
    };
    void hideCursor() override
    {
        glfwSetInputMode(mGLFWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mCursorEnabled = false;
    };
    bool isCursorShown() override { return mCursorEnabled; };
    // void close() { glfwDestroyWindow(mGLFWindow); }
    GLFWwindow *mGLFWindow; // pointer need it by glfw ---> this is ugly
  private:
    int mWidth;
    int mHeight;
    std::string mTitle;
    Timer mTimer;
    bool mCursorEnabled;
};
} // namespace Nova
