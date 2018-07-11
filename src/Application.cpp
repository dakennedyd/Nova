#include <thread>
#include "logger/Logger.h"
#include "Application.h"
#include "ECS/Entity.h"
#include "Crc.h"
#ifdef NOVA_WINDOWS_PLATFORM
	#include "windows/FileSystem.h"
	#include "windows/Window.h"
#else
	#include "linux/FileSystem.h"
	#include "linux/Window.h"
#endif
#include "Settings.h"
#include "graphics/opengl/GraphicsSystem.h"
#include "resource_manager/ResourceManager.h"
#include "ECS/DefaultSystems.h"
#include "graphics/RendererFrontend.h"
#include "graphics/opengl/RendererBackendDeferred.h"
#include "Init.h"

namespace Nova {	
	class Entity;
	Entity & Application::World::createEntity(const std::string & name)
	{
		//note: id alwasys has to be a SID of name
		uint32_t id = SID(name.c_str());
		/*reminder: emplace returns a pair consisting of an iterator to the inserted element,
		or the already-existing element if no insertion happened, and a bool
		denoting whether the insertion took place. True for Insertion, False for No Insertion. */
		Entity &entity = mEntities.emplace(std::make_pair(id, Entity(name, id))).first->second;
		addToWorldTree(entity);
		//entity.addComponent<TransformComponent>();
		return entity;
	}

	void Application::World::destroyEntity(Entity & entity)
	{		
		while (!entity.getChildren().empty())
		//for (auto e : entity.getChildren())
		{			
			destroyEntity(*(entity.getChildren().begin()->second));
			//destroyEntity(*(entity.getChildren().begin()->second));
			//entity.getChildren().erase(entity.getChildren().begin());
		}

		//unsubscribe entity from all systems
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

	Entity & Application::World::getEntity(const std::string & name)
	{
		//return mEntities.at(SID(name.c_str()));
		if (mEntities.find(SID(name.c_str())) != mEntities.end()) //does the entity exist?
		{
			return mEntities.find(SID(name.c_str()))->second;
		}
		else {
			LOG_WARNING("trying to get a non existent entity with name:" + name);
			return mNoEntity;
		}
	}

	Entity & Application::World::getEntity(const uint32_t key)
	{
		return mEntities.at(key);
	}

	void Application::World::attachEntities(Entity & father, Entity & child,
		PropagationType type)
	{
		//just some sanity checks so that the program doesn't accidentaly
		//rip a hole in the universe..
		if (father.getID() == child.getID()) return;
		Entity *entity = &father;
		//checks if the attachment will create a loop(which could hang the thread)
		while (entity->mParent)
		{
			if (entity->mParent->getID() == child.getID()) return;
			entity = entity->mParent;
		}

		if (child.mParent) //if child already has a parent detach it
		{
			detachEntity(child);
		}		
		deleteFromWorldTree(child);
		father.mChildren.emplace(std::make_pair(child.getID(), &child));
		child.mParent = &father;
		father.mTransform.propagationType = type;
	}

	void Application::World::detachEntity(Entity & child)
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
		for (auto &keyEntityPair : mWorldTree)
		{
			keyEntityPair.second->setFinalTransformAndPropagate(Mat4::makeIdentityMatrix());
		}
		/*processing the entities comes afterwards because camera gets its parameters
		from the final transform - this needs to be fixed because rendering comes one frame after
		inputs are detected (need to think of a nice way of fixing this) */
		for (auto &typeSystemPair : mSystems)
		{
			typeSystemPair.second->processEntities();
		}
	}

	void Application::World::deleteFromWorldTree(const Entity & entity)
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

	void Application::World::addToWorldTree(Entity & entity)
	{
		//auto it = std::find(mWorldTree.begin(), mWorldTree.end(), entity.getID());
		//if (it != mWorldTree.end())// does entity exist in mWorldTree?
		//{		
		//}
		//else {
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
			#endif
			EngineSettings::getInstance().startUp();
			Window::getInstance().startUp();
			InputSystem::getInstance().startUp();
			GraphicsSystem::getInstance().startUp();
			ResourceManager::getInstance().startUp();

			//registers application to get keyboard events
			InputSystem::getInstance().getKeyboard().registerObserver(this);

			//registers systems to the ECS 
			mWorld.registerSystem<VisualSystem>();
			mWorld.registerSystem<RotationSystem>();
			mWorld.registerSystem<PlayerInputSystem>();
			mWorld.registerSystem<CameraSystem>();
			mWorld.registerSystem<LightSystem>();

			//LOG_INFO("Initialization took:" << Timer::getTimeSinceEngineStart() << "ms.");
			mIsInitialized = true;
			setKeyCallback([]() {if (InputSystem::getInstance().getKeyboard().getKeyState(Keys::KEY_ESC)) { Application::getInstance().shutDown(); }});

		}
	}
	void Application::startMainLoop()
	{		
		if (mIsInitialized)
		{
			//sets the current renderer
			GraphicsSystem::getInstance().setRendererFrontend(std::make_shared<RendererFrontend>());
			GraphicsSystem::getInstance().setRendererBackend(std::make_shared<RendererBackendDeferred>());
			auto& rendererBackend = GraphicsSystem::getInstance().getRendererBackend();
			auto& rendererFrontend = GraphicsSystem::getInstance().getRendererFrontend();
			
			long targetFPS = EngineSettings::getInstance().getInteger("Video", "fps");
			long targetFrameTime = 1000000 / (targetFPS * 1000);
			int64_t timeDelta = SIMULATION_TIME_STEP + 1;
			
			auto &window = Window::getInstance();
			auto &input = InputSystem::getInstance();
			auto &mouse = input.getMouse();
			long frameTime = 0, fps = 0;
			//Timer reloj;
			window.show();
			rendererFrontend.createRenderPackets();
			Timer clock;
			while (!isClosing() && !window.isClosing()) {
				clock.reset();			
				input.processInputs();
				while (timeDelta >= SIMULATION_TIME_STEP)
				{
					mWorld.update();		//updates all entities in the world					
					timeDelta -= SIMULATION_TIME_STEP;

					mouse.mPreviousX = mouse.mX; mouse.mPreviousY = mouse.mY; //needed so that mouse.getMotionVector() can work
					mouse.wheel = 0;
				}				

				rendererBackend.render();
				window.swapFrameBuffers();
				frameTime = static_cast<long>(clock.getMillis());
				std::this_thread::sleep_for(std::chrono::milliseconds(targetFrameTime - frameTime)); //UUUUUGGGGGLLLYYYYYY!!!
				fps =static_cast<long>(1000.0 / clock.getMillis());
				window.setTitle(NOVA_DESCRIPTION_STRING + "| FPS:" + std::to_string(fps) +" Frame time:" + std::to_string(frameTime) + "ms.");
				timeDelta += clock.getMillis();
			}
		}
	}
	void Application::shutDown()
	{
		if (mIsInitialized)
		{
			this->mIsClosing = true;
			ResourceManager::getInstance().shutDown();
			GraphicsSystem::getInstance().shutDown();
			InputSystem::getInstance().shutDown();
			Window::getInstance().shutDown();
			EngineSettings::getInstance().shutDown();
			#ifdef LOG_ACTIVE
			Logger::getInstance().shutDown();
			#endif
			FileSystem::getInstance().shutDown();
			mIsInitialized = false;
		}
	}
}