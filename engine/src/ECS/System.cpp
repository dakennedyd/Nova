
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

#include "ECS/System.h"
#include "ECS/Entity.h"
#include "logger/Logger.h"
#include <utility>

namespace Nova
{
void System::registerEntity(Entity &entity)
{
    // entity.mSubscribedSystems.insert(std::make_pair(std::type_index(typeid(this)), this));
    /*if (std::find(entity.mSubscribedSystems.begin(), entity.mSubscribedSystems.end(), this) !=
    entity.mSubscribedSystems.end())
    {
            entity.mSubscribedSystems.push_back(this);
    }*/
    if (mRegisteredEntities.insert(std::make_pair(entity.getID(), &entity))
            .second) // did and insertion actually happen?
    {
        onRegister(&entity);
    }
    else
        LOG_WARNING("failed to register entity " << entity.getName());
}
void System::unregisterEntity(Entity &entity)
{
    auto it = mRegisteredEntities.find(entity.getID());
    if (it != mRegisteredEntities.end()) // does the entity exist in the system's list?
    {
        onUnregister(&entity);
        // entity.mSubscribedSystems.erase(std::type_index(typeid(this)));
        // entity.mSubscribedSystems.erase(this);
        mRegisteredEntities.erase(it);
    }
    else
        LOG_WARNING("failed to unregister entity " << entity.getName());
}
void System::processEntities()
{
    for (auto &idEntityPair : mRegisteredEntities)
    {
        processEntity(idEntityPair.second);
    }
}
} // namespace Nova