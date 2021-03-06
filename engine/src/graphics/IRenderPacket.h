
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
#include "graphics/Bindable.h"
#include "graphics/IGPUProgramParameter.h"

namespace Nova
{
class IRenderPacket : public Bindable
{
  public:
    IRenderPacket() = default;
    virtual ~IRenderPacket() = default;

    virtual void addParameter(IGPUProgramParameter *parameter) = 0;
    virtual void updateAllUniforms() const = 0;
    virtual void updateCamera() const =0;
    virtual void draw() const = 0;
    virtual uint64_t getID() const = 0;
    virtual void setUniform(const std::string &name, const float value) const = 0;
    virtual void setUniform(const std::string &name, const int value) const = 0;
    virtual void setUniform(const std::string &name, const Mat4 &value) const = 0;
};
} // namespace Nova
