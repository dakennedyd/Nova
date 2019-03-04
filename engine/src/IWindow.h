
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
#include <string>
namespace Nova
{

/**
 * Interface class for Window
 * this class would manage windowing functions for diferent OSes
 * right now it uses just glfw
 */
class IWindow
{
  public:
    IWindow(){};
    virtual ~IWindow(){};
    // virtual void windowLoop() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual bool isClosing() = 0;
    virtual const std::string &getTitle() = 0;
    virtual void setTitle(const std::string &title) = 0;
    virtual void swapFrameBuffers() = 0;
    virtual void processEvents() = 0;
    virtual void minimize() = 0;
    virtual void restore() = 0;
    virtual void show() = 0;
    virtual void showCursor() = 0;
    virtual void hideCursor() = 0;
    virtual bool isCursorShown() = 0;
};
} // namespace Nova
