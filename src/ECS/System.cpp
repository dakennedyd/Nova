#include <utility>
#include "ECS/System.h"
#include "ECS/Entity.h"

namespace Nova{	
	void System::registerEntity(Entity & entity)
	{
		//entity.mSubscribedSystems.insert(std::make_pair(std::type_index(typeid(this)), this));
		/*if (std::find(entity.mSubscribedSystems.begin(), entity.mSubscribedSystems.end(), this) != entity.mSubscribedSystems.end())
		{
			entity.mSubscribedSystems.push_back(this);
		}*/
		if ( mRegisteredEntities.insert(std::make_pair(entity.getID(), &entity)).second ) //did and insertion actually happen?
		{
			onRegister(&entity);
		}
	}
	void System::unregisterEntity(Entity & entity)
	{
		auto it = mRegisteredEntities.find(entity.getID());
		if (it != mRegisteredEntities.end()) //does the entity exist in the system's list?
		{
			onUnregister(&entity);
			//entity.mSubscribedSystems.erase(std::type_index(typeid(this)));
			//entity.mSubscribedSystems.erase(this);
			mRegisteredEntities.erase(it);
		}
	}
	void System::processEntities()
	{
		for (auto & idEntityPair : mRegisteredEntities)
		{
			processEntity(idEntityPair.second);
		}
	}
}