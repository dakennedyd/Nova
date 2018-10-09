
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
//#include "ECS/Entity.h"
#include "ISingleton.h"
#include "ISubSystem.h"
//#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "logger/Logger.h"
#include "math/Quaternion.h"
#include "math/Vector.h"
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <vector>

namespace Nova
{
class UnitQuat;
enum PhysicalShape
{
    CUBE,
    SPHERE,
    CAPSULE,
    CONE,
    PLANE
};
struct PhysicsObject
{
    // PhysicsObject() = default;
    // PhysicsObject(PhysicsObject const &) = delete;            // Copy ctor
    // PhysicsObject(PhysicsObject &&) = default;                // Move ctor
    // PhysicsObject &operator=(PhysicsObject const &) = delete; // Copy assign
    // PhysicsObject &operator=(PhysicsObject &&) = delete;      // Move assign
    // ~PhysicsObject()
    // {
    //     // delete shape;
    //     // delete motionState;
    //     // delete bodyInfo;
    //     // delete body;
    // }

    btCollisionShape *shape;
    btTransform transform;
    btScalar mass;
    btVector3 localInertia;
    btDefaultMotionState *motionState;
    btRigidBody::btRigidBodyConstructionInfo *bodyInfo;
    btRigidBody *body;
    uint64_t *id;
};

struct PhysicsTransform
{
    Vec3 translation;
    UnitQuat rotation;
};

struct PhysicalContactCallback : public btCollisionWorld::ContactResultCallback
{
    PhysicalContactCallback(const std::function<void(int, int)> callback) : callback(callback) {}
    btScalar addSingleResult(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap,
                             int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap,
                             int partId1, int index1) override;
    std::function<void(int, int)> callback;
};

struct CollisionAction
{
    uint32_t id;
    // void *callback;
    PhysicalContactCallback *callback;
    // std::function<void(int, int)> callback;
    btCollisionObject *body;
};

class Physics : public ISingleton<Physics>, public ISubSystem
{
  public:
    Physics() = default;
    ~Physics() = default;
    void startUp() override;
    void shutDown() override;

    void addObject(const uint64_t id, const PhysicalShape shape, const Vec3 &dimensions,
                   const Vec3 &scale, const Vec3 &translation, const UnitQuat &rotation,
                   const float mass, const float friction, const float restitution);
    void removeObject(const uint64_t id);
    void simulate(const float timeStep);

    /**
     * @brief add a callback for when a entity with id "id" collides with any other object
     *
     * @param id of the entity that colliding with any other entity
     * @param callback that defines the action to take when the object collides
     */
    void addContactCallback(uint64_t id, std::function<void(int, int)> &callback)
    {
        // PhysicalContactCallback *c = new PhysicalContactCallback(callback);
        // mCollisionActions.emplace_back(std::make_pair(id, new
        // PhysicalContactCallback(callback)));
        CollisionAction ca;
        ca.id = id;
        ca.body = mObjects.at(id).body;
        ca.body->setCollisionFlags(
            ca.body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK /*|
            btCollisionObject::CF_NO_CONTACT_RESPONSE*/);
        // PhysicalContactCallback *pcc = new PhysicalContactCallback(callback);
        ca.callback = new PhysicalContactCallback(callback);
        mCollisionActions.push_back(ca);
    }
    void removeContactCallback(u_int64_t id)
    {
        auto pred = [id](const CollisionAction &item) { return item.id == id; };
        // auto a = std::find_if(std::begin(mCollisionActions), std::end(mCollisionActions), pred)
        // != std::end(mCollisionActions);
        auto it = std::find_if(std::begin(mCollisionActions), std::end(mCollisionActions), pred);

        // mCollisionActions.erase(std::remove(mCollisionActions.begin(), mCollisionActions.end(),
        // it),mCollisionActions.end());

        if (it != mCollisionActions.end())
        {
            delete it->callback;
            std::swap(*it, mCollisionActions.back());
            mCollisionActions.pop_back();
        }
    }

    PhysicsTransform getObjectTransform(const uint64_t id);

  private:
    // necessary for bullet init
    btDefaultCollisionConfiguration *mCollisionConfiguration;
    btCollisionDispatcher *mDispatcher;
    btBroadphaseInterface *mBroadPhase;
    btSequentialImpulseConstraintSolver *mSolver;
    btDiscreteDynamicsWorld *mDynamicsWorld;

    // btAlignedObjectArray<std::pair<uint64_t, PhysicsObject>> mObjects;
    // std::vector<std::pair<uint64_t, PhysicsObject>> mObjects;
    std::unordered_map<uint64_t, PhysicsObject> mObjects;

    std::vector<CollisionAction>
        mCollisionActions; // stores the action callbacks, id and the collisionObject
};

} // namespace Nova