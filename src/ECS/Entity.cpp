
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

#include "ECS/Entity.h"
#include "Crc.h"
#include "math/Matrix.h"
#include "math/Quaternion.h"
#include "math/Vector.h"
//#include <string>

namespace Nova
{
Entity::Entity(const std::string &name) : mName(name), mEntityID(SID(name.c_str())) {}

Entity::Entity(const std::string &name, const uint32_t id) : mName(name), mEntityID(id) {}

Entity::~Entity() {}

/*void Entity::attachChild(Entity & child)
{
        //mChildren.push_back(&child);
        deleteFromWorldTree(child);
        mChildren.emplace(std::make_pair(child.getID(), &child));
        child.mParent = this;
}

void Entity::detachChild(Entity & child)
{
        auto it = mChildren.find(child.getID());
        if (it != mChildren.end()) //does the child exist?
        {
                child.mParent = nullptr;
                mChildren.erase(it);
                //addToWorldTree
        }
}*/

void Entity::rotate(const Vec3 axis, float angle)
{
    mTransform.rotation = UnitQuat(axis, angle) * mTransform.rotation;
    mTransform.forward.rotateSelf(axis, angle);
    mTransform.right.rotateSelf(axis, angle);
    mTransform.up.rotateSelf(axis, angle);
}

void Entity::rotate(const UnitQuat &rotation)
{
    /*mTransform.rotation = rotation * mTransform.rotation;
    mTransform.forward.rotateSelf(rotation);
    mTransform.right.rotateSelf(rotation);
    mTransform.up.rotateSelf(rotation);*/

    Vec3 forward{0.0f, 0.0f, -1.0f};
    Vec3 right{1.0f, 0.0f, 0.0f};
    Vec3 up{0.0f, 1.0f, 0.0f};

    mTransform.rotation = mTransform.rotation * rotation;
    mTransform.forward = forward.rotate(mTransform.rotation * rotation);
    mTransform.right = right.rotate(mTransform.rotation * rotation);
    mTransform.up = up.rotate(mTransform.rotation * rotation);
}

void Entity::setRotation(const UnitQuat &rotation)
{
    Vec3 forward{0.0f, 0.0f, -1.0f};
    Vec3 right{1.0f, 0.0f, 0.0f};
    Vec3 up{0.0f, 1.0f, 0.0f};

    mTransform.rotation = rotation;
    mTransform.forward = forward.rotate(rotation);
    mTransform.right = right.rotate(rotation);
    mTransform.up = up.rotate(rotation);

    /*mTransform.right = mTransform.forward ^ mTransform.up;
    mTransform.right.normalizeSelf();
    mTransform.up = mTransform.right ^ mTransform.forward;
    mTransform.up.normalizeSelf();*/
}

void Entity::setPosition(const Vec3 &position) { mTransform.translation = position; }

void Entity::setScale(const Vec3 &scale) { mTransform.scale = scale; }

void Entity::setScale(float scale) { mTransform.scale = scale; }

void Entity::move(const Vec3 &vector) { mTransform.translation = mTransform.translation + vector; }

void Entity::setFinalTransformAndPropagate(const Mat4 &propagatedTransform)
{
    Mat4 transform; // (Mat4::makeIdentityMatrix());
    if (mTransform.propagationType == PropagationType::POSITION_ONLY)
    {
        transform = Mat4::makeTranslationMatrix(mTransform.translation);
        mTransform.finalTransform = Mat4::makeScalingMatrix(mTransform.scale) *
                                    mTransform.rotation.toRotationMatrix4() * transform *
                                    propagatedTransform;
    }
    else if (mTransform.propagationType == PropagationType::POSITION_ROTATION)
    {
        transform = mTransform.rotation.toRotationMatrix4() *
                    Mat4::makeTranslationMatrix(mTransform.translation);
        mTransform.finalTransform =
            Mat4::makeScalingMatrix(mTransform.scale) * transform * propagatedTransform;
    }
    else if (mTransform.propagationType == PropagationType::POSITION_ROTATION_SCALING)
    {
        transform = mTransform.rotation.toRotationMatrix4() *
                    Mat4::makeTranslationMatrix(mTransform.translation) *
                    Mat4::makeScalingMatrix(mTransform.scale);
        mTransform.finalTransform = transform * propagatedTransform;
    }
    Mat3 pt = propagatedTransform.toMat3();
    // mTransform.translation = pt * mTransform.translation;
    /*mTransform.forward = pt * mTransform.forward;
    mTransform.right = pt * mTransform.right;
    mTransform.up = pt * mTransform.up;*/

    mTransform.finalTranslation = mTransform.finalTransform.getTranslation();
    Mat4 finalPropagatedTransform{transform * propagatedTransform};
    // TODO: should not calculate normal for transform that don't need them
    mTransform.normalMatrix = mTransform.finalTransform.calcNormalMatrix();
    for (auto &keyEntityPair : mChildren)
    {
        keyEntityPair.second->setFinalTransformAndPropagate(finalPropagatedTransform);
    }
}

/*void Entity::setFinalTransform()
{
        Mat4 transform(mTransform.rotation.toRotationMatrix4()
                * Mat4::makeTranslationMatrix(mTransform.translation)
                * Mat4::makeScalingMatrix(mTransform.scale));

        //this is not perfect frame by frame but seems to work fine
        //at 75fps
        if (mParent)
        {
                mTransform.finalTransform = transform * mParent->getFinalTransform();
        }
        else {
                mTransform.finalTransform = transform;
        }
        mTransform.normal = mTransform.finalTransform.calcNormalMatrix();
}*/

/*void Entity::setTransform(const Mat4 & transform)
{
        mTransform.transform = transform;
}*/

/*void Entity::addComponent(IComponent * component)
{
        //mComponents[std::type_index(typeid(component))] = component;
        mComponents.insert(std::make_pair(
                std::type_index(typeid(component)),
                component));
}*/
/*void Entity::subscribeTo(System * system)
{
        mSubscribedSystems.insert(std::make_pair(
                std::type_index(typeid(system)),
                system));
}*/
} // namespace Nova