
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

#include "Application.h"
#include "Crc.h"
//#include "ECS/Entity.h"
#include "PreInit.h"
#include "logger/Logger.h"
#ifdef NOVA_WINDOWS_PLATFORM
#    include "windows/FileSystem.h"
#    include "windows/Window.h"
#elif defined NOVA_LINUX_PLATFORM
#    include "linux/FileSystem.h"
#    include "linux/Window.h"
#endif
#include "ECS/DefaultSystems.h"
#include "Init.h"
#include "Settings.h"
#include "graphics/RendererFrontend.h"
#ifdef NOVA_OPENGL
#    include "graphics/opengl/GraphicsSystem.h"
#    include "graphics/opengl/RendererBackendDeferred.h"
#endif
#include "Physics.h"
#include "audio/Audio.h"
#include "graphics/DebugUI.h"
#include "resource_manager/ResourceManager.h"
#include <thread>
//***********************TEMPORARY CODE DELETE ASAP*****************************
#include "graphics/opengl/TextureCube.h"
//***********************TEMPORARY CODE DELETE ASAP*****************************

namespace Nova
{
class Entity;
Entity &Application::World::createEntity(const std::string &name)
{
    // note: id alwasys has to be a SID of name
    uint32_t id = SID(name.c_str());
    /*reminder: emplace returns a pair consisting of an iterator to the inserted
    element, or the already-existing element if no insertion happened, and a
    bool denoting whether the insertion took place. True for Insertion, False
    for No Insertion. */
    Entity &entity = mEntities.emplace(std::make_pair(id, Entity(name, id))).first->second;
    addToWorldTree(entity);
    // entity.addComponent<TransformComponent>();
    return entity;
}

void Application::World::destroyEntity(Entity &entity)
{
    while (!entity.getChildren().empty())
    // for (auto e : entity.getChildren())
    {
        destroyEntity(*(entity.getChildren().begin()->second));
        // destroyEntity(*(entity.getChildren().begin()->second));
        // entity.getChildren().erase(entity.getChildren().begin());
    }

    // unsubscribe entity from all systems
    for (auto it : mSystems)
    {
        it.second->unregisterEntity(entity);
    }

    if (entity.mParent)
    {
        entity.mParent->mChildren.erase(entity.getID());
    }
    deleteFromWorldTree(entity);
    mEntities.erase(entity.getID());
}

Entity &Application::World::getEntity(const std::string &name)
{
    // return mEntities.at(SID(name.c_str()));
    auto it = mEntities.find(SID(name.c_str()));
    if (it != mEntities.end()) // does the entity exist?
    {
        return it->second;
    }
    else
    {
        LOG_WARNING("trying to get a non existent entity with name:" + name);
        return mNoEntity;
    }
}

Entity &Application::World::getEntity(const uint32_t key) { return mEntities.at(key); }

void Application::World::attachEntities(Entity &father, Entity &child, PropagationType type)
{
    // just some sanity checks so that the program doesn't accidentaly
    // rip a hole in the universe..
    if (father.getID() == child.getID()) return;
    Entity *entity = &father;
    // checks if the attachment will create a loop(which could hang the thread)
    while (entity->mParent)
    {
        if (entity->mParent->getID() == child.getID()) return;
        entity = entity->mParent;
    }

    if (child.mParent) // if child already has a parent detach it
    {
        detachEntity(child);
    }
    deleteFromWorldTree(child);
    father.mChildren.emplace(std::make_pair(child.getID(), &child));
    child.mParent = &father;
    father.mTransform.propagationType = type;
}

void Application::World::detachEntity(Entity &child)
{
    if (child.mParent)
    {
        child.mParent->mChildren.erase(child.getID());
        child.mParent = nullptr;
        addToWorldTree(child);
    }
}

void Application::World::update()
{
    // calculate the final transform for all root entities in the world and propagate that transform
    // to their children
    for (auto &keyEntityPair : mWorldTree)
    {
        keyEntityPair.second->setFinalTransformAndPropagate(Mat4::makeIdentityMatrix());
    }

    // do processEntities for each entity registered with a system
    for (auto &typeSystemPair : mSystems)
    {
        typeSystemPair.second->processEntities();
    }
}

void Application::World::deleteFromWorldTree(const Entity &entity)
{
    /*auto it = std::find(mWorldTree.begin(), mWorldTree.end(), entity.getID());
    if (it != mWorldTree.end())// does entity exist in mWorldTree?
    {
            mWorldTree.erase(it);
    }*/
    if (!entity.mParent)
    {
        mWorldTree.erase(entity.getID());
    }
}

void Application::World::addToWorldTree(Entity &entity)
{
    // auto it = std::find(mWorldTree.begin(), mWorldTree.end(),
    // entity.getID()); if (it != mWorldTree.end())// does entity exist in
    // mWorldTree?
    //{
    //}
    // else {
    mWorldTree.emplace(std::make_pair(entity.getID(), &entity));
    //}
}

void Application::startUp()
{
    if (!mIsInitialized)
    {
        FileSystem::getInstance().startUp();
#ifdef LOG_ACTIVE
        Logger::getInstance().startUp();
        LOG_INFO("Excutable path is " << PATH_TO_BINARY);
#endif
        EngineSettings::getInstance().startUp();
        Physics::getInstance().startUp();
        Window::getInstance().startUp();
        InputSystem::getInstance().startUp();
        ResourceManager::getInstance().startUp();
        GraphicsSystem::getInstance().startUp();
        DebugUI::getInstance().startUp();
        Audio::getInstance().startUp();

        // registers application to get keyboard events
        InputSystem::getInstance().getKeyboard().registerObserver(this);

        // registers systems to the ECS
        mWorld.registerSystem<VisualSystem>();
        mWorld.registerSystem<RotationSystem>();
        mWorld.registerSystem<PlayerInputSystem>();
        mWorld.registerSystem<CameraSystem>();
        mWorld.registerSystem<LightSystem>();
        mWorld.registerSystem<PhysicalSystem>();
        mWorld.registerSystem<SoundSystem>();

        /* this is just a basic key callback it is meant to be replaced with your own function
        when you press the ESC key it exits the program */
        setKeyCallback([]() {
            if (InputSystem::getInstance().getKeyboard().getKeyState(Keys::KEY_ESC))
            {
                // Application::getInstance().shutDown(); //need to fix: shutdown launches and
                // exception for some reason
                std::exit(EXIT_SUCCESS);
            }
        });

        // creates a camera entity and set it as the default camera entity
        Entity &defaultCamera = getWorld().createEntity("Default Camera");
        defaultCamera.setPosition(Vec3(0.0f, 0.0f, 2.0f));
        defaultCamera.addComponent<CameraComponent>(Mat4::makePerspectiveMatrix(
            toRadians(60.0f),
            static_cast<float>(EngineSettings::getInstance().getInteger("Video", "width")) /
                EngineSettings::getInstance().getInteger("Video", "height"),
            0.1f, 300.0f));
        getWorld().GetSystem<CameraSystem>()->registerEntity(defaultCamera);
        GraphicsSystem::getInstance().setCurrentCamera(&defaultCamera);

        // sets the current renderer
        auto &gs = GraphicsSystem::getInstance();
        gs.setRendererFrontend(std::make_shared<RendererFrontend>());
        gs.setRendererBackend(std::make_shared<RendererBackendDeferred>());
        gs.getRendererBackend().init();

        LOG_INFO("Initialization took:" << Timer::getTimeSinceEngineStart() << "ms.");
        this->mIsInitialized = true;
    }
}
void Application::startMainLoop()
{
    if (this->mIsInitialized)
    {
        auto &rendererBackend = GraphicsSystem::getInstance().getRendererBackend();
        // auto &rendererFrontend = GraphicsSystem::getInstance().getRendererFrontend();

        long targetFPS = EngineSettings::getInstance().getInteger("Video", "fps");
        long targetFrameTime = 1000000 / (targetFPS * 1000);
        int64_t timeDelta = SIMULATION_TIME_STEP + 1; // in milliseconds

        auto &window = Window::getInstance();
        auto &input = InputSystem::getInstance();
        auto &mouse = input.getMouse();
        long frameTime = 0; //, fps = 0; //, entityUpdateTime, renderTime;
        //***********************TEMPORARY CODE DELETE ASAP*****************************
        //std::shared_ptr<TextureCube> tc;
        //***********************TEMPORARY CODE DELETE ASAP*****************************

        window.show();
        Timer frameTimeClock, renderClock, entityUpdateClock, soundCleanUp;
        while (!this->isClosing() && !window.isClosing())
        {
            frameTimeClock.reset();
            input.processInputs();
            while (timeDelta >= SIMULATION_TIME_STEP)
            {
                entityUpdateClock.reset();
                mWorld.update(); // updates all entities in the world
                Physics::getInstance().simulate(1.0f / timeDelta);

                mProfileTimes["Entities update"] = entityUpdateClock.getMicro();
                timeDelta -= SIMULATION_TIME_STEP;

                mouse.mPreviousX = mouse.mX;
                mouse.mPreviousY = mouse.mY; // needed so that mouse.getMotionVector() can work
                mouse.wheel = 0;
                Audio::getInstance().updateListenerData();
            }
            // sync physics sim with the graphics system
            for (auto &IDEntityPair : mWorld.GetSystem<PhysicalSystem>()->getEntities())
            {
                auto &e = *IDEntityPair.second;
                auto t = Physics::getInstance().getObjectTransform(e.getID());
                e.setPosition(t.translation);
                e.setRotation(t.rotation);
            }
            if (soundCleanUp.getMillis() > 500)
            {
                Audio::getInstance().cleanUpSources();
                soundCleanUp.reset();
            }
            // Physics::getInstance().simulate(1.0f / 30.0f);

            renderClock.reset();
            rendererBackend.render();
            mProfileTimes["Render time"] = renderClock.getMicro();
            window.swapFrameBuffers();
            // glFinish();
            frameTime = frameTimeClock.getMillis();

            auto s = FileSystem::getInstance().checkIfShadersChanged();
            for (auto &shaderFile : s)
            {
                // LOG_INFO("Recompiling shader:" << shaderFile);
                ResourceManager::getInstance().get<GPUProgram>(shaderFile)->recompile();

                //***********************TEMPORARY CODE DELETE ASAP*****************************
                // std::vector<void*> textureData;
                // if (shaderFile == "shaders/noise_cube")
                // {
                //     // tc = std::make_shared<TextureCube>(textureData, 1024, 1024);
                //     // rendererBackend.drawToTextureCube(
                //     //     ResourceManager::getInstance().get<GPUProgram>("shaders/noise_cube"), tc);
                // }
                //***********************TEMPORARY CODE DELETE ASAP*****************************
            }

            std::this_thread::sleep_for(
                std::chrono::milliseconds(targetFrameTime - frameTime)); // UUUUUGGGGGLLLYYYYYY!!!
            // fps = 1000.0 / frameTimeClock.getMillis();
            // window.setTitle(NOVA_DESCRIPTION_STRING + "| FPS:" + std::to_string(fps) +
            //                 " frametime:" + std::to_string(frameTime) +
            //                 "ms. render:" + std::to_string(renderTime) +
            //                 " us. logic:" + std::to_string(entityUpdateTime) + " us.");
            timeDelta += frameTimeClock.getMillis();
        }
        this->shutDown();
    }
}
void Application::shutDown()
{
    if (this->mIsInitialized)
    {
        this->mIsClosing = true;
        Audio::getInstance().shutDown();
        DebugUI::getInstance().shutDown();
        GraphicsSystem::getInstance().shutDown();
        ResourceManager::getInstance().shutDown();
        InputSystem::getInstance().shutDown();
        // Window::getInstance().shutDown();
        Physics::getInstance().shutDown();
        EngineSettings::getInstance().shutDown();
#ifdef LOG_ACTIVE
        Logger::getInstance().shutDown();
#endif
        FileSystem::getInstance().shutDown();
        this->mIsInitialized = false;
    }
    // int a;
    // std::cin >> a;
}
} // namespace Nova