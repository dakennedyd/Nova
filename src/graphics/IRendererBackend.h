
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
//#include <vector>
#include <unordered_map>
namespace Nova
{
struct PBRSkybox;
class IRendererBackend
{
  public:
    IRendererBackend() = default;
    virtual ~IRendererBackend() = default;
    virtual void init() = 0;
    virtual void render() = 0;
    virtual void setSkyBox(const std::shared_ptr<PBRSkybox> &skyBox) = 0;
    virtual std::unordered_map<std::string, long> &getProfileTimes() = 0;
    // virtual void setSkyBox(std::shared_ptr<TextureCube> skyBox) = 0;
    // virtual void setIBLData(std::shared_ptr<IBL_Data> data) = 0; NOO
    // virtual std::uint64_t addLight(ILight light) = 0;
    // virtual std::uint64_t addPacket(IRenderPacket packet) = 0;
    /*note: parameter is not a reference becuse RenderPacket is not copyable
    so insted uses the move constructor*/
    // virtual std::uint64_t addPacket(RenderPacket primitive) = 0;
    // virtual void setSkyBox(RenderPacket *skyBox) = 0;
    //virtual void physicsDebugDraw() = 0;
    virtual void drawLine(const Vec3 &from, const Vec3 &to, const Vec3 &color) = 0;
};

} // namespace Nova