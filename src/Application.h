#pragma once
#include "ISingleton.h"
#ifdef NOVA_WINDOWS_PLATFORM
	#include "windows/InputSystem.h"
#else
	#include "linux/InputSystem.h"
#endif
#include "ECS/Entity.h"

#include "ECS/System.h"

namespace Nova {
	//class Entity;
	class Application final :public ISingleton<Application>, public IKeyboardObserver
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
			Entity& createEntity(const std::string &name);
			/** removes entity if it exists
			* if the entity has atached children they get deleted too
			*/
			void destroyEntity(Entity & entity);
			//void destroyEntity(const std::string &name);

			Entity& getEntity(const std::string &name);
			Entity& getEntity(const uint32_t key);

			void attachEntities(Entity& father, Entity& child, PropagationType type = PropagationType::POSITION_ROTATION);
			void detachEntity(Entity& child);
			/*adds a system to the world.
			If a system with the same type already exist in world then it does nothing*/
			template<typename T>
			void registerSystem()
			{
				static_assert(std::is_base_of<System, T>::value,
					"Invalid System: a valid system has to be a subclass of System");
				mSystems.emplace(std::make_pair(std::type_index(typeid(T)), new T));
			}

			/*Gets a system of the type specified. If the type specified does not exist then
			it launches an exception*/
			template<typename T>
			T* GetSystem() { return (static_cast<T*>(mSystems.at(std::type_index(typeid(T))))); }
			void update();			

		private:
			Entity mNoEntity{ "NOVA_INVALID_ENTITY" };
			std::unordered_map<uint32_t, Entity> mEntities;
			std::unordered_map<std::type_index, System*> mSystems; //only one of each system subtype is allowed				
			std::unordered_map<uint32_t, Entity*> mWorldTree;			

			void deleteFromWorldTree(const Entity & entity);
			void addToWorldTree(Entity & entity);
		};
	public:
		Application() = default;
		~Application() = default;
		void startUp();						//intialize the engine
		void startMainLoop();
		void shutDown();
		bool isClosing() { return mIsClosing; };
		//Window mWindow;
		World & getWorld() { return mWorld; };
		void setKeyCallback(void(*callback)()) { mCallback = callback; };
		void onKeyPress() override { if (mCallback) mCallback(); };
		World mWorld;
	protected:
		//Timer mTimer;
		bool mIsClosing = false;
		bool mIsInitialized = false;
		void(*mCallback)() = nullptr;
	};

}