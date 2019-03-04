
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
#include <typeindex>
#include <unordered_map>

namespace Nova
{
class Entity;

class System
{
  public:
    System() = default;
    virtual ~System() = default;

    /*Register and entity into the system. If the entity is already
    registered it does nothing*/
    void registerEntity(Entity &entity);
    /*Unregister the entity from the system. if the entity is already unregistered then it does
     * nothing*/
    void unregisterEntity(Entity &entity);
    void processEntities();

    /*Adds a component to the list of required components of a system,
    if a component of the same type already exist then it does nothing*/
    template <typename T> void addRequiredComponent()
    {
        static_assert(std::is_base_of<IComponent, T>::value,
                      "Invalid Component: a valid component has to be a subclass of IComponent");
        mRequiredComponents.emplace(std::make_pair(std::type_index(typeid(T)), new T));
    }

    auto &getEntities() { return mRegisteredEntities; }

  protected:
    virtual void processEntity(Entity *entity) = 0;
    virtual void onRegister(Entity *entity) = 0;
    virtual void onUnregister(Entity *entity) = 0;

  private:
    std::unordered_map<uint32_t, Entity *> mRegisteredEntities;
    // stores only the components that are of interest to this system(only one of each type of
    // component is allowed)
    std::unordered_map<std::type_index, IComponent *> mRequiredComponents;
};

} // namespace Nova