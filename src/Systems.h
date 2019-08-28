#pragma once
//#include "../engine/src/ECS/System.h"
#include <Nova.h>

namespace Nova{
class Entity;
int bulletCounter = 0;
int timer = 0;

struct ProjectileComponent final : public IComponent
{
    ProjectileComponent() = delete;
    ProjectileComponent(float speed, int timeToLive)
    : speed(speed), lifetime(timeToLive){};
    float speed;
    int lifetime; //in msecs
};

struct FireComponent final : public IComponent
{
    FireComponent() = delete;
    FireComponent(const int fireRate)
        : fireRate(fireRate){};
    int fireRate;
};

class ProjectileSystem final : public Nova::System
{
    void onRegister(Entity *entity) override
    {
        //entity->getComponent<ProjectileComponent>().dir.normalizeSelf();
    }

    void processEntity(Entity *entity) override
    {
        ProjectileComponent &projComponent = entity->getComponent<Nova::ProjectileComponent>();
        entity->move(entity->getTransformStruct().forward * projComponent.speed);
        projComponent.lifetime--;
        if(projComponent.lifetime < 1)
        {
            //auto &world = Application::getInstance().getWorld();
            //world.destroyEntity(*entity);
            entity->markForDeletion();
        }
    }

    void onUnregister(Entity *entity) override {}
};

class FireSystem final : public Nova::System
{
    void onRegister(Entity *entity) override {}

    void processEntity(Entity *entity) override
    {
        //FireComponent &fireComponent = entity->getComponent<Nova::FireComponent>();
        auto &is = InputSystem::getInstance();
        timer++;
        if (is.getActionStatus(SID("fire")) && timer > 10)
        {
            timer = 0;
            auto &engine = Application::getInstance();
            auto &rm = ResourceManager::getInstance();

            bulletCounter++;
            Entity &bullet = engine.getWorld().createEntity("bullet" + std::to_string(bulletCounter));
            //bullet.setScale(1.0f);
            bullet.setPosition(entity->getTransformStruct().finalTranslation);
            bullet.setRotation(entity->getRotation());
            bullet.setScale(Vec3(0.5f,0.25f,7.0f));

            bullet.addComponent<VisualComponent>(rm.get<Mesh>("meshes/cube"),
                                                rm.get<Material>("materials/aluminium"));
            engine.getWorld().getSystem<VisualSystem>()->registerEntity(bullet);
            bullet.addComponent<LightComponent>(LightType::POINT_LIGHT, Vec3(0.0f, 200.0f, 0.0f), false);
            engine.getWorld().getSystem<LightSystem>()->registerEntity(bullet);
            bullet.addComponent<ProjectileComponent>(1,500);
            engine.getWorld().getSystem<ProjectileSystem>()->registerEntity(bullet);
            bullet.addComponent<PhysicalComponent>(0.0f, PhysicalShape::CUBE, Vec3(0.25f,0.125f,0.5f));
            // , 1.5f, 0.5f, [&](int id1, int id2) {
            //     //engine.getWorld().getEntity(id2).playSound(rm.get<SoundBuffer>("sounds/ping"));
            //     //engine.getWorld().getEntity("ship").playSound(rm.get<SoundBuffer>("sounds/ping"));
            //     auto &engine = Application::getInstance();
            //     auto &e1 = engine.getWorld().getEntity(id1);
            //     auto &e2 = engine.getWorld().getEntity(id2);
            //     // if(e1.getID() != entity->getID())
            //     // {
            //     //     e1.markForDeletion();
            //     // }
            // });
            //engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(bullet);

            entity->playSound(rm.get<SoundBuffer>("sounds/laser"));
        }
    }

    void onUnregister(Entity *entity) override {}
};

}