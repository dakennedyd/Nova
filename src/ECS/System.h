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