
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

#include "Physics.h"
#include "Application.h"
#include "logger/Logger.h"
#include "math/Matrix.h"
#include "math/Quaternion.h"
//#include "math/Vector.h"
#include "Crc.h"
#include <string>

namespace Nova
{
void Physics::startUp()
{
    /// collision configuration contains default setup for memory, collision setup.
    mCollisionConfiguration = new btDefaultCollisionConfiguration();

    /// use the default collision dispatcher.
    mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

    // broad phase collision detector
    mBroadPhase = new btDbvtBroadphase();

    /// the default constraint solver.
    mSolver = new btSequentialImpulseConstraintSolver;

    mDynamicsWorld =
        new btDiscreteDynamicsWorld(mDispatcher, mBroadPhase, mSolver, mCollisionConfiguration);

    mDynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

    //===================================================================

    // contactCallback = new PhysicalContactCallback();
    // limitPlane = new PhysicsObject;
    // limitPlane->shape = new btStaticPlaneShape(btVector3(0.0, 1.0, 0.0), -15.0);
    // // limitPlane->shape = new btBoxShape(btVector3(20.0, .1, 20.0));
    // limitPlane->mass = 0;
    // limitPlane->transform.setIdentity();
    // limitPlane->motionState = new btDefaultMotionState(limitPlane->transform);
    // limitPlane->bodyInfo = new btRigidBody::btRigidBodyConstructionInfo(
    //     limitPlane->mass, limitPlane->motionState, limitPlane->shape, btVector3(0.0, 0.0, 0.0));
    // limitPlane->body = new btRigidBody(*limitPlane->bodyInfo);
    // mDynamicsWorld->addRigidBody(limitPlane->body);
    // limitPlane->body->setCollisionFlags(limitPlane->body->getCollisionFlags() |
    //                                     btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK |
    //                                     btCollisionObject::CF_NO_CONTACT_RESPONSE);
}
void Physics::shutDown()
{
    delete mDynamicsWorld;
    delete mSolver;
    delete mBroadPhase;
    delete mDispatcher;
    delete mCollisionConfiguration;
}

void Physics::addObject(const uint64_t id, const PhysicalShape shape, const Vec3 &dimensions,
                        const Vec3 &scale, const Vec3 &translation, const UnitQuat &rotation,
                        const float mass, const float friction, const float restitution)
{
    PhysicsObject object;
    if (shape == PhysicalShape::PLANE)
    {
        object.shape = new btStaticPlaneShape(btVector3(0.0, 0.0, -1.0), -0.5);
    }
    if (shape == PhysicalShape::CUBE)
    {
        object.shape =
            new btBoxShape(btVector3(dimensions.getX(), dimensions.getY(), dimensions.getZ()));
        object.shape->setLocalScaling(btVector3(scale.getX(), scale.getY(), scale.getZ()));
        // object.shape = new btBoxShape(btVector3(10.0f, 0.1f, 10.0f));
    }
    if (shape == PhysicalShape::SPHERE)
    {
        object.shape = new btSphereShape(dimensions.getX());
        object.shape->setLocalScaling(btVector3(scale.getX(), scale.getY(), scale.getZ()));
    }
    if (shape == PhysicalShape::CAPSULE)
    {
        object.shape = new btCapsuleShape(dimensions.getX(), dimensions.getY());
        object.shape->setLocalScaling(btVector3(scale.getX(), scale.getY(), scale.getZ()));
    }
    if (shape == PhysicalShape::CONE)
    {
        object.shape = new btConeShape(dimensions.getX(), dimensions.getY());
        object.shape->setLocalScaling(btVector3(scale.getX(), scale.getY(), scale.getZ()));
    }
    // object.transform.setFromOpenGLMatrix(transform);
    object.transform.setOrigin(
        btVector3(translation.getX(), translation.getY(), translation.getZ()));
    object.transform.setRotation(
        btQuaternion(rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW()));
    object.mass = mass;
    // object.localInertia = btVector3(localInertia.getX(), localInertia.getY(),
    // localInertia.getZ());
    if (object.mass != 0.0f) object.shape->calculateLocalInertia(object.mass, object.localInertia);

    // using motionstate is optional, it provides interpolation capabilities, and only
    // synchronizes 'active' objects
    object.motionState = new btDefaultMotionState(object.transform);
    object.bodyInfo = new btRigidBody::btRigidBodyConstructionInfo(
        object.mass, object.motionState, object.shape, object.localInertia);
    object.bodyInfo->m_friction = friction;
    object.bodyInfo->m_restitution = restitution;
    object.body = new btRigidBody(*object.bodyInfo);
    // object.body->setFriction(1.f);
    // object.body->setActivationState(DISABLE_DEACTIVATION);
    object.body->applyCentralImpulse(btVector3(0.0, 0.0, 2.0));
    // object.body->setRollingFriction(.001);
    // object.body->setSpinningFriction(0.001);
    // object.body->setAnisotropicFriction(object.shape->getAnisotropicRollingFrictionDirection(),
    //                                     btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);
    // object.body->setUserIndex(id);

    // flag object as kinematic
    // body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

    // object.body->setUserIndex(id);
    // object.body->setUserIndex2(id);
    object.id = new uint64_t{id};
    object.body->setUserPointer(object.id);
    // add the body to the dynamics world
    mDynamicsWorld->addRigidBody(object.body);
    // mObjects.emplace_back(std::pair<uint64_t, PhysicsObject>(id, std::move(object)));
    mObjects[id] = object;
    // object.body->setCollisionFlags(object.body->getCollisionFlags() |
    //                                btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
}

PhysicsTransform Physics::getObjectTransform(const uint64_t id)
{
    PhysicsTransform result;
    // auto r = mObjects.at(id).transform;
    btTransform btr;
    mObjects.at(id).motionState->getWorldTransform(btr);
    result.translation =
        Vec3(btr.getOrigin().getX(), btr.getOrigin().getY(), btr.getOrigin().getZ());
    result.rotation = UnitQuat(btr.getRotation().getX(), btr.getRotation().getY(),
                               btr.getRotation().getZ(), btr.getRotation().getW());
    return result;

    // for (auto &object : mObjects)
    // {
    //     if (object.first == id) // shitty search for object with matching id
    //     {
    //         // object.second.transform.getOpenGLMatrix(data);
    //         btTransform trans;
    //         object.second.body->getMotionState()->getWorldTransform(trans);
    //         float *data = new float[16];
    //         trans.getOpenGLMatrix(data);
    //         return data;
    //     }
    // }

    // mDynamicsWorld->stepSimulation(1.0f / 60.0f);
    // // int numCollisionObjects = mDynamicsWorld->getNumCollisionObjects();
    // // for (int i = 0; i < numCollisionObjects; i++)
    // // {
    // //     btCollisionObject *colObj = mDynamicsWorld->getCollisionObjectArray()[i];
    // //     btRigidBody *body = btRigidBody::upcast(colObj);
    // //     int a = colObj->getUserIndex();
    // //     if (body->getUserIndex() == id)
    // //     {
    // //         btTransform trans;
    // //         if (body && body->getMotionState())
    // //         {
    // //             body->getMotionState()->getWorldTransform(trans);
    // //         }
    // //         else
    // //         {
    // //             trans = colObj->getWorldTransform();
    // //         }
    // //         float *data = new float[16];
    // //         trans.getOpenGLMatrix(data);
    // //         return data;
    // //     }
    // // }
    // LOG_ERROR("can't find physical object with id:" << id);
    // return nullptr;
}
void Physics::removeObject(const uint64_t id)
{
    // for (auto &object : mObjects)
    // {
    //     if (object.first == id) // shitty search for object with matching id
    //     {
    //         object.first == -1;
    //         // delete object.second.body;
    //     }
    // }
    mDynamicsWorld->removeRigidBody(mObjects.at(id).body);

    delete mObjects.at(id).shape;
    delete mObjects.at(id).motionState;
    delete mObjects.at(id).bodyInfo;
    delete mObjects.at(id).id;
    delete mObjects.at(id).body;
    mObjects.erase(id);
}

void Physics::simulate(const float timeStep)
{
    mDynamicsWorld->stepSimulation(timeStep);
    for (auto &collisionAction : mCollisionActions)
    {
        mDynamicsWorld->contactTest(collisionAction.body, *collisionAction.callback);
    }
}

btScalar PhysicalContactCallback::addSingleResult(btManifoldPoint &cp,
                                                  const btCollisionObjectWrapper *colObj0Wrap,
                                                  int partId0, int index0,
                                                  const btCollisionObjectWrapper *colObj1Wrap,
                                                  int partId1, int index1)
{
    uint32_t id1, id2;
    // if (colObj0Wrap->getCollisionObject()->getUserPointer())
    //{
    // ugly cast incoming!
    id1 = *((uint32_t *)(((btRigidBody *)colObj0Wrap->getCollisionObject())->getUserPointer()));
    id2 = *((uint32_t *)(((btRigidBody *)colObj1Wrap->getCollisionObject())->getUserPointer()));
    callback(id1, id2);
    // LOG_DEBUG("Contact:" << entityName);

    // Application::getInstance().getWorld().destroyEntity(
    //     Application::getInstance().getWorld().getEntity(id));

    // ((btRigidBody *)(colObj0Wrap->getCollisionObject()))
    //     ->applyCentralImpulse(btVector3(0.0f, 1.0f, 0.0f));
    //}
    return 0;
}
} // namespace Nova