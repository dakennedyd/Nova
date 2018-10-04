
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
#include "graphics/opengl/RendererInit.h"
#include <memory>
//#include "graphics/opengl/FrameBuffer.h"
#include "IComponent.h"
#include "Physics.h"
#include "math/Matrix.h"
#include "math/Vector.h"

namespace Nova
{
class Mesh;
class Material;
class FrameBuffer;

struct VisualComponent final : public IComponent
{
    VisualComponent() = delete;
    VisualComponent(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Material> &material)
        : mesh(mesh), material(material){};
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    uint64_t packetID;
};

struct RotationComponent final : public IComponent
{
    float speed = 0; // in RPM
    Vec3 axis;
};

struct MovementComponent final : public IComponent
{
    float speed = 1.0f;
};

struct CameraComponent final : public IComponent
{
    CameraComponent() = delete;
    CameraComponent(const Mat4 &projMatrix) : projection(projMatrix){};
    Mat4 projection;
    Mat4 view;
};

struct LightComponent final : public IComponent
{
    LightComponent() = delete;
    LightComponent(const LightType &lightType, const Vec3 &color, const bool castsShadows = false)
        : type(lightType), color(color)
    {
    }
    std::shared_ptr<FrameBuffer> shadowFramebuffer;
    bool castsShadow;
    LightType type;
    Vec3 color;
    uint64_t lightID;
};

struct PhysicalComponent final : public IComponent
{
    PhysicalComponent() = delete;
    PhysicalComponent(float mass, PhysicalShape shape, Vec3 dimensions,
                      Vec3 localInertia = Vec3(0.0f))
        : mass(mass), shape(shape),
          dimensions(dimensions.getX(), dimensions.getY(), dimensions.getZ()),
          localInertia(localInertia.getX(), localInertia.getY(), localInertia.getZ())
    {
    }
    float mass;
    PhysicalShape shape;
    Vec3 dimensions;
    Vec3 localInertia;
    // friction coeficient?
};
} // namespace Nova
