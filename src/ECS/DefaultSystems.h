
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
/*This are systems(ECS model) that extends the system class
most of them are created for convenience as they are
common enough to be necessary in any game*/
#include "Crc.h"
#include "ECS/DefaultComponents.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "Init.h"
#include "PreInit.h"
#include "math/miscellaneous.h"
#ifdef NOVA_WINDOWS_PLATFORM
#    include "windows/InputSystem.h"
#elif defined NOVA_LINUX_PLATFORM
#    include "linux/InputSystem.h"
#endif
#ifdef NOVA_OPENGL
#    include "graphics/opengl/FrameBuffer.h"
#    include "graphics/opengl/GraphicsSystem.h"
#endif

namespace Nova
{
class VisualSystem final : public System
{
    void processEntity(Entity *entity) override {}

    void onRegister(Entity *entity) override
    {
        // VisualComponent& vc = entity->GetComponent<VisualComponent>();
        // vc.renderPacketID =
        // GraphicsSystem::getInstance().getRendererBackend().addPacket(RenderPacket{ vc.mesh,
        // vc.material, &entity->getTransformStruct() });
    }

    void onUnregister(Entity *entity) override
    {
        // VisualComponent& vc = entity->GetComponent<VisualComponent>();
        // GraphicsSystem::getInstance().getRendererBackend().removePacket(vc.renderPacketID);
    }
};

class LightSystem final : public System
{
    void processEntity(Entity *entity) override {}

    void onRegister(Entity *entity) override
    {
        LightComponent &lc = entity->GetComponent<LightComponent>();
        if (lc.castsShadow)
        {
            lc.shadowFramebuffer =
                std::make_shared<FrameBuffer>(FrameBuffer::makeShadowMapFrameBuffer());
        }
    }

    void onUnregister(Entity *entity) override
    {
        LightComponent &lc = entity->GetComponent<LightComponent>();
        if (lc.castsShadow)
        {
            lc.shadowFramebuffer = nullptr;
        }
    }
};

class RotationSystem final : public System
{
    void processEntity(Entity *entity) override
    {
        RotationComponent &rotComponent = entity->GetComponent<RotationComponent>();
        Vec4 v4{rotComponent.axis.getX(), rotComponent.axis.getY(), rotComponent.axis.getZ(), 0.0f};
        // v4.normalizeSelf();
        v4 = entity->getFinalTransform() * v4;
        Vec3 v3(v4.getX(), v4.getY(), v4.getZ());
        v3.normalize();
        float t = 1.0f / (1.0f / SIMULATION_TIME_STEP * 1000.0f);
        // float rotationsPerSeconds = rotComponent.speed / 60.0f;
        entity->rotate(v3, toRadians(rotComponent.speed * 6 * t)); // RPM to degrees
        // entity->rotate(rotComponent.axis, toRadians(rotComponent.speed));
    }

    void onRegister(Entity *entity) override
    {
        entity->GetComponent<RotationComponent>().axis.normalizeSelf();
    }

    void onUnregister(Entity *entity) override {}
};

class PlayerInputSystem final : public System
{
    void processEntity(Entity *entity) override
    {
        auto &ts = entity->getTransformStruct();
        auto &is = InputSystem::getInstance();
        auto &mouse = is.getMouse();
        Vec3 mouseVector = mouse.getMotionVector();

        // Vec3 mouseVector2 = ts.finalTransform.toMat3() * mouseVector;
        ////entity->getTransformStruct().forward;
        // Vec3 dir = mouseVector2 ^ entity->getTransformStruct().forward;
        // dir.normalizeSelf();

        MovementComponent &m = entity->GetComponent<MovementComponent>();
        UnitQuat pitch(-mouseVector.getY() * mouse.getSensitivity(), 0.0f, 0.0f);
        UnitQuat yaw(0.0f, mouseVector.getX() * mouse.getSensitivity(), 0.0f);

        // entity->rotate(UnitQuat(-mouseVector.y* mouse.getSensitivity(), mouseVector.x*
        // mouse.getSensitivity(), 0.0f)); entity->rotate(dir, toRadians(mouseVector2.getMagnitud()
        // * mouse.getSensitivity()));

        /*camera orientation is separated in pitch and yaw and multiplied
        in this specific order(yaw*orientation*pitch) to prevent camera tilt*/
        entity->setRotation(yaw * entity->getRotation() * pitch);
        // entity->rotate(pitch*yaw);
        float t = 1.0f / (1.0f / SIMULATION_TIME_STEP * 1000.0f);
        if (is.getActionStatus(SID("move_forward")))
        {
            entity->move(ts.forward * m.speed * t);
        }
        if (is.getActionStatus(SID("move_backward")))
        {
            entity->move(ts.forward * -m.speed * t);
        }
        if (is.getActionStatus(SID("strafe_right")))
        {
            entity->move(ts.right * -m.speed * t);
        }
        if (is.getActionStatus(SID("strafe_left")))
        {
            entity->move(ts.right * m.speed * t);
        }
        if (is.getActionStatus(SID("move_up")))
        {
            entity->move(ts.up * m.speed * t);
        }
        if (is.getActionStatus(SID("move_down")))
        {
            entity->move(ts.up * -m.speed * t);
        }
        if (is.getActionStatus(SID("turn_left")))
        {
            entity->rotate(ts.up, -m.speed * t);
        }
        if (is.getActionStatus(SID("turn_right")))
        {
            entity->rotate(ts.up, m.speed * t);
        }
        if (is.getActionStatus(SID("turn_up")))
        {
            entity->rotate(ts.right, m.speed * t);
        }
        if (is.getActionStatus(SID("turn_down")))
        {
            entity->rotate(ts.right, -m.speed * t);
        }
        if (is.getActionStatus(SID("tilt_left")))
        {
            entity->rotate(ts.forward, m.speed * t);
        }
        if (is.getActionStatus(SID("tilt_right")))
        {
            entity->rotate(ts.forward, -m.speed * t);
        }
    }

    void onRegister(Entity *entity) override {}

    void onUnregister(Entity *entity) override {}
};

class CameraSystem final : public System
{
    void processEntity(Entity *entity) override
    {
        CameraComponent &camera = entity->GetComponent<CameraComponent>();
        auto &ts = entity->getTransformStruct();
        Vec3 pos{ts.finalTransform.getTranslation()};
        pos = pos * -1.0f;
        Mat3 rot{ts.finalTransform.toMat3()};
        rot = rot.transpose();
        // auto& parentTransform = entity->getParent().getTransformStruct();

        camera.view = Mat4::makeLookAtMatrix(pos, pos - rot * ts.forward, rot * ts.up);
        // camera.view = Mat4::makeLookAtMatrix(ts.translation, ts.translation - ts.forward, ts.up);
        // camera.view = Mat4::makeLookAtMatrix(ts.translation, ts.translation + ts.forward, ts.up);
    }

    void onRegister(Entity *entity) override
    {
        CameraComponent &camera = entity->GetComponent<CameraComponent>();

        CameraInfo cameraInfo;
        cameraInfo.projectionMatrix = &(camera.projection);
        cameraInfo.viewMatrix = &(camera.view);
        // entity->getFinalTransform().getTranslation()
        cameraInfo.position = &(entity->getNonConstTransformStruct().finalTranslation);
        cameraInfo.forwardVector = &(entity->getNonConstTransformStruct().forward);

        // passes camera information(position view matrix etc. so the renderer can see it)
        GraphicsSystem::getInstance().setCurrentCamera(cameraInfo);
    }

    void onUnregister(Entity *entity) override {}
};
} // namespace Nova
