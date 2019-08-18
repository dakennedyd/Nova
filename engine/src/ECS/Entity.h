
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
#include "ECS/IComponent.h"
#include "audio/SoundBuffer.h"
#include "logger/Logger.h"
#include "math/Matrix.h"
#include "math/Quaternion.h"
#include "math/Vector.h"
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace Nova
{
enum PropagationType
{
    POSITION_ONLY,
    POSITION_ROTATION,
    POSITION_ROTATION_SCALING
};

struct Transform
{
    Transform()
        : finalTransform(Mat4::makeIdentityMatrix()),
          /*transform(Mat4::makeIdentityMatrix()),*/ normalMatrix(
              finalTransform.calcNormalMatrix()){};
    Transform(const Mat4 &transform)
        : finalTransform(transform),
          /*transform(Mat4::makeIdentityMatrix()),*/ normalMatrix(transform.calcNormalMatrix()){};

    Vec3 scale = Vec3(1.0f);
    UnitQuat rotation;
    Vec3 translation;

    Vec3 forward{0.0f, 0.0f, -1.0f};
    Vec3 right{1.0f, 0.0f, 0.0f};
    Vec3 up{0.0f, 1.0f, 0.0f};

    Mat4 finalTransform;
    Vec3 finalTranslation; // useful for the lights
    Mat4 normalMatrix;
    enum PropagationType propagationType = PropagationType::POSITION_ROTATION_SCALING;
};

class System;
// class SoundBuffer;
/*don't create entities on your own use the world object*/
class Entity
{
    friend class Application;
    friend class System;

  public:
    ~Entity();
    Entity(const Entity &) = delete;             // copy ctor
    Entity(Entity &&other) = default;            // move ctor
    Entity &operator=(Entity const &) = delete;  // copy assignment op
    Entity &operator=(Entity &&other) = default; // move assignment op

    /*Adds a new component to an entity, if a component of the same type already exist
    then it does nothing*/
    template <typename T, typename... Params> void addComponent(Params &&... parameters)
    {
        static_assert(
            std::is_base_of<IComponent, T>::value,
            "Invalid Component: a valid component has to be a subclass of IComponent class");
        mComponents.emplace(
            std::make_pair(std::type_index(typeid(T)), new T(std::forward<Params>(parameters)...)));
    }

    /*Gets a component of the type specified. If the type specified does not exist then
    it launches an error*/
    template <typename T> T &getComponent()
    {
        static_assert(
            std::is_base_of<IComponent, T>::value,
            "Invalid Component: a valid component has to be a subclass of IComponent class");
        // if (!containsComponent<T>())
        // {
        //     LOG_ERROR("trying to get non existent component");
        //     std::exit(1);
        // }
        return *(static_cast<T *>(mComponents.at(std::type_index(typeid(T)))));
    }

    template <typename T> bool containsComponent() const
    {
        // auto search = mComponents.find(std::type_index(typeid(T)));
        if (mComponents.find(std::type_index(typeid(T))) != mComponents.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /*-----------------FIX THIS MESS!!------------------------------*/
    template <typename T> void removeComponent() { mComponents.erase(std::type_index(typeid(T))); }

    /*template<typename T>
    void subscribeTo()
    {
            static_assert(std::is_base_of<System, T>::value,
                    "Invalid System: a valid system has to be a subclass of System class");
            World::GetSystem<T>()->registerEntity(this);
            mSubscribedSystems.insert(std::make_pair(std::type_index(typeid(T)),
    World::GetSystem<T>()));
    };*/
    // void registerEntity();		//notifies the relevant systems based on the entity's
    // components

    const uint32_t getID() const { return mEntityID; };
    const std::string &getName() const { return mName; };

    // void attachChild(Entity &child);
    // void detachChild(Entity &child);
    const Transform &getTransformStruct() const { return mTransform; };
    Transform &getNonConstTransformStruct() { return mTransform; };

    /*rotates the entity by the provided axis and angle
    note: if the axis is not normalized the behaivior is undefined*/
    void rotate(const Vec3 axis, float angle);
    void rotate(const UnitQuat &rotation);
    void setRotation(const UnitQuat &rotation);
    const UnitQuat &getRotation() { return mTransform.rotation; };
    void setPosition(const Vec3 &position);
    void setScale(const Vec3 &scale);
    void setScale(float scale);
    // void setWorldPosition(const Vec3 &vector);
    void move(const Vec3 &vector);
    // void scale(const Vec3 &scale);
    const std::unordered_map<uint32_t, Entity *> &getChildren() const { return mChildren; };
    const Entity &getParent() const { return *mParent; };
    const Mat4 &getFinalTransform() const { return mTransform.finalTransform; };
    void playSound(const std::shared_ptr<SoundBuffer> soundBuffer);
    void stopSound();

    void markForDeletion() {mMarkedForDeletion = true;}
    bool isMarkedForDeletion() {return mMarkedForDeletion;}

  private:
    void setFinalTransformAndPropagate(const Mat4 &propagatedTransform);
    // void setFinalTransform();
    Entity(const std::string &name);
    /*id always has to be a hash(SID) of name*/
    Entity(const std::string &name,
           const uint32_t id); // this ctor is only so that World doesn't have to generate id twice
    std::string mName;
    uint32_t mEntityID;
    std::unordered_map<std::type_index, IComponent *>
        mComponents; // only one of each type of component is allowed
    // std::vector<System*> mSubscribedSystems;	//to make entity destruction faster
    Transform mTransform;
    // std::vector<Entity*> mChildren;
    std::unordered_map<uint32_t, Entity *> mChildren;
    Entity *mParent = nullptr;
    bool mMarkedForDeletion = false;
};

} // namespace Nova