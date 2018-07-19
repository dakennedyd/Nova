
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
#include "ISingleton.h"
#include "PreInit.h"
#ifdef NOVA_WINDOWS_PLATFORM
#    include "windows/InputSystem.h"
#elif defined NOVA_LINUX_PLATFORM
#    include "linux/InputSystem.h"
#endif
#include "ECS/Entity.h"
#include "ECS/System.h"
#include <functional>

namespace Nova
{
// class Entity;
class Application final : public ISingleton<Application>, public IKeyboardObserver
{
    friend class Entity;

  private:
    class World
    {
        friend class Entity;

      public:
        World() = default;
        ~World() = default;

        /*creates a new entity.
        if an entity with the same name already exist returns that one instead*/
        Entity &createEntity(const std::string &name);

        /** removes entity if it exists
         * if the entity has atached children they get deleted too
         */
        void destroyEntity(Entity &entity);
        // void destroyEntity(const std::string &name);

        Entity &getEntity(const std::string &name);
        Entity &getEntity(const uint32_t key);

        void attachEntities(Entity &father, Entity &child,
                            PropagationType type = PropagationType::POSITION_ROTATION);
        void detachEntity(Entity &child);

        /*adds a system to the world.
        If a system with the same type already exist in world then it does
        nothing*/
        template <typename T> void registerSystem()
        {
            static_assert(std::is_base_of<System, T>::value,
                          "Invalid System: a valid system has to be a subclass "
                          "of System");
            mSystems.emplace(std::make_pair(std::type_index(typeid(T)), new T));
        }

        /*Gets a system of the type specified. If the type specified does not
        exist then it launches an exception*/
        template <typename T> T *GetSystem()
        {
            return (static_cast<T *>(mSystems.at(std::type_index(typeid(T)))));
        }
        void update();

      private:
        Entity mNoEntity{"NOVA_INVALID_ENTITY"};
        std::unordered_map<uint32_t, Entity> mEntities;
        std::unordered_map<std::type_index, System *>
            mSystems; // only one of each system subtype is allowed
        std::unordered_map<uint32_t, Entity *> mWorldTree;

        void deleteFromWorldTree(const Entity &entity);
        void addToWorldTree(Entity &entity);
    };

  public:
    Application() = default;
    ~Application() = default;
    void startUp(); // intialize the engine
    void startMainLoop();
    void shutDown();
    bool isClosing() { return mIsClosing; };
    // Window mWindow;
    World &getWorld() { return mWorld; };

    // https://stackoverflow.com/questions/32840369/no-matching-function-
    // error-when-passing-lambda-function-as-argument#32840595
    template <typename F> void setKeyCallback(F callback) { mCallback = callback; }
    // void setKeyCallback(void (*callback)()) { mCallback = callback; };
    void onKeyPress() override
    {
        if (mCallback) mCallback();
    };
    World mWorld;

  protected:
    // Timer mTimer;
    bool mIsClosing = false;
    bool mIsInitialized = false;
    std::function<void()> mCallback;
    // void (*mCallback)() = nullptr;
};

} // namespace Nova