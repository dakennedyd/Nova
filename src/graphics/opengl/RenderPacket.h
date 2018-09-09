
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
#include "ECS/Entity.h"
#include "graphics/IRenderPacket.h"
#include "graphics/opengl/Material.h"
#include "graphics/opengl/RenderPacket.h"

namespace Nova
{
class Mesh;
struct Transform;
class RenderPacket : public IRenderPacket
{
  public:
    RenderPacket() = delete;
    RenderPacket(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Material> &material,
                 const Transform &transform);
    RenderPacket(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Material> &material);
    ~RenderPacket();
    RenderPacket(const RenderPacket &) = delete;            // copy ctor
    RenderPacket(RenderPacket &&other);                     // move ctor
    RenderPacket &operator=(RenderPacket const &) = delete; // copy assignment op
    RenderPacket &operator=(RenderPacket &&other);          // move assignment op

    const std::shared_ptr<Material> &getMaterial() const { return mMaterial; };
    const std::shared_ptr<Mesh> &getMesh() const { return mMesh; };

    void addParameter(IGPUProgramParameter *parameter) override
    {
        mParameters.push_back(parameter);
    };
    void updateAllUniforms() const override;
    void draw() const override;

    void bind() const override;
    void unBind() const override;

    uint64_t getID() const override { return mID; };

  private:
    std::shared_ptr<Mesh> mMesh = nullptr;
    std::shared_ptr<Material> mMaterial = nullptr;
    std::vector<IGPUProgramParameter *> mParameters;
    uint64_t mID;
    static uint64_t count;
};
} // namespace Nova
