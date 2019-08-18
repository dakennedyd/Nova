#include <string>

#include "Nova.h"
#include "PlanetGeneration.h"
#include "Systems.h"

using namespace Nova;

int main()
{
    auto &engine = Application::getInstance();
    engine.startUp();
    auto &rm = ResourceManager::getInstance();
    engine.getWorld().registerSystem<ProjectileSystem>();
    engine.getWorld().registerSystem<FireSystem>();

    bool skyboxBool = false;
    bool rotatePlanet = true;
    bool changeCamera = false;
    bool light = true;
    int objCount = 0;    

    engine.setKeyCallback([&]() {
        auto &keyboard = InputSystem::getInstance().getKeyboard();
        auto &world = Application::getInstance().getWorld();
        if (keyboard.getKeyState(Keys::KEY_T))
        {
            Entity &moon = world.getEntity("star");
            moon.move(Vec3(0.0f, 0.1f, 0.0f));
        }
        if (keyboard.getKeyState(Keys::KEY_Y))
        {
            Entity &moon = world.getEntity("star");
            moon.move(Vec3(0.0f, -0.1f, 0.0f));
        }
        if (keyboard.getKeyState(Keys::KEY_1))
        {
            skyboxBool = !skyboxBool;
            if (skyboxBool)
            {
                GraphicsSystem::getInstance().getRendererBackend().setSkyBox(
                    rm.get<PBRSkybox>("textures/skyboxes/fireSky"));
            }
            else
            {
                GraphicsSystem::getInstance().getRendererBackend().setSkyBox(
                    rm.get<PBRSkybox>("textures/skyboxes/space"));
            }
        }
        if (keyboard.getKeyState(Keys::KEY_2))
        {
            if (rotatePlanet)
            {
                rotatePlanet = false;
                Entity &floor = world.getEntity("floor");
                floor.stopSound();
            }
            else
            {
                rotatePlanet = true;
            }
        }
        if (keyboard.getKeyState(Keys::KEY_F1))
        {
            if (changeCamera)
            {
                changeCamera = false;
                engine.getWorld().GetSystem<CameraSystem>()->unregisterEntity(
                    engine.getWorld().getEntity("fpsCamera"));
                engine.getWorld().GetSystem<PlayerInputSystem>()->unregisterEntity(
                    engine.getWorld().getEntity("ship"));
                engine.getWorld().GetSystem<CameraSystem>()->registerEntity(
                    engine.getWorld().getEntity("Default Camera"));
                Window::getInstance().showCursor();
            }
            else
            {
                changeCamera = true;
                engine.getWorld().GetSystem<CameraSystem>()->registerEntity(
                    engine.getWorld().getEntity("fpsCamera"));
                engine.getWorld().GetSystem<PlayerInputSystem>()->registerEntity(
                    engine.getWorld().getEntity("ship"));
                engine.getWorld().GetSystem<CameraSystem>()->unregisterEntity(
                    engine.getWorld().getEntity("Default Camera"));
                GraphicsSystem::getInstance().setCurrentCamera(
                    &engine.getWorld().getEntity("fpsCamera"));
                Window::getInstance().hideCursor();
            }
        }
        if (keyboard.getKeyState(Keys::KEY_F))
        {
            Random rnd;
            objCount++;
            Entity &thing = engine.getWorld().createEntity("thing" + std::to_string(objCount));
            auto &camEnti = engine.getWorld().getEntity("ship");
            thing.setPosition(camEnti.getTransformStruct().finalTranslation);
            thing.addComponent<SoundComponent>(false, 5.0f, 1.0f);
            engine.getWorld().GetSystem<SoundSystem>()->registerEntity(thing);
            if (rnd.nextDouble() > .5)
            {
                thing.setScale(1.0f);
                thing.addComponent<VisualComponent>(rm.get<Mesh>("meshes/cube"),
                                                    rm.get<Material>("materials/yellow_plastic"));
                thing.addComponent<PhysicalComponent>(
                    1.0f, PhysicalShape::CUBE, thing.getTransformStruct().scale / 4.0f, 1.5f, 0.5f);
            }
            else
            {
                thing.setScale(0.5f);
                thing.addComponent<VisualComponent>(rm.get<Mesh>("meshes/dragon"),
                                                    rm.get<Material>("materials/aluminium"));
                thing.addComponent<PhysicalComponent>(1.0f, PhysicalShape::CUBE,
                                                      Vec3(.5f, .7f, .25f), 1.5f, 0.5f);
            }
            engine.getWorld().GetSystem<VisualSystem>()->registerEntity(thing);
            engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(thing);

            Physics::getInstance().pushObject(thing.getID(),
                                              camEnti.getTransformStruct().forward * 5.0f);
            if (true)
            {
                thing.addComponent<LightComponent>(LightType::POINT_LIGHT, Vec3(1000.0), false);
                engine.getWorld().GetSystem<LightSystem>()->registerEntity(thing);
            }
        }
        if (keyboard.getKeyState(Keys::KEY_R))
        {
            {
                world.destroyEntity(world.getEntity("thing" + std::to_string(objCount)));
                if (objCount > 0) objCount--;
            }
        }
        if (keyboard.getKeyState(Keys::KEY_5))
        {
            if (light)
            {
                world.GetSystem<LightSystem>()->unregisterEntity(world.getEntity("light1"));
                light = false;
            }
            else
            {
                world.GetSystem<LightSystem>()->registerEntity(world.getEntity("light1"));
                light = true;
            }
        }
    });
    engine.setMouseButtonCallback([&]() {
        // auto &mouse = InputSystem::getInstance().getMouse();
        // auto &is = InputSystem::getInstance();
        // //if (keyboard.getKeyState(Keys::KEY_C))
        // //if (mouse.getButtonState(Keys::MOUSE_LEFT))
        // if (is.getActionStatus(SID("fire")))
        // {
        //     projCount++;
        //     Entity &projectile = engine.getWorld().createEntity("projectile" + std::to_string(projCount));
        //     projectile.setPosition(engine.getWorld().getEntity("ship").getTransformStruct().finalTranslation);
        //     projectile.setRotation(engine.getWorld().getEntity("ship").getRotation());
        //     projectile.addComponent<ProjectileComponent>(0.1,5000);
        //     engine.getWorld().GetSystem<ProjectileSystem>()->registerEntity(projectile);
        //     projectile.addComponent<VisualComponent>(rm.get<Mesh>("meshes/cube"),
        //                                             rm.get<Material>("materials/yellow_plastic"));
        //     engine.getWorld().GetSystem<VisualSystem>()->registerEntity(projectile);

        //     // projectile.addComponent<PhysicalComponent>(
        //     //         0.0f, PhysicalShape::CUBE, projectile.getTransformStruct().scale / 4.0f, 1.5f, 0.5f);
        //     // engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(projectile);
        // }
    });

    Entity &fpsCamera = engine.getWorld().createEntity("fpsCamera");
    fpsCamera.setPosition(Vec3(0.0f, 0.5f, 2.5f));
    fpsCamera.addComponent<CameraComponent>(Mat4::makePerspectiveMatrix(
        toRadians(60.0f),
        static_cast<float>(EngineSettings::getInstance().getInteger("Video", "width")) /
            EngineSettings::getInstance().getInteger("Video", "height"),
        0.5f, 800.0f));
    engine.getWorld().GetSystem<CameraSystem>()->registerEntity(
        engine.getWorld().getEntity("fpsCamera"));
    fpsCamera.addComponent<MovementComponent>();
    fpsCamera.getComponent<MovementComponent>().speed = 10.0f;

    Entity &floor = engine.getWorld().createEntity("floor");
    floor.setRotation(UnitQuat(Vec3(1.0f, 0.0f, 0.0f), toRadians(90.0f)));
    floor.addComponent<VisualComponent>(rm.get<Mesh>("meshes/floor"),
                                        rm.get<Material>("materials/orange_plastic"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(floor);
    floor.setPosition(Vec3(0.0f, 0.0f, 0.0f));
    floor.addComponent<PhysicalComponent>(0.0f, PhysicalShape::CUBE, Vec3(5.0f, 5.0f, 0.001f), 1.5f,
                                          0.5f);
    engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(floor);

    Entity &planeOfDestruction = engine.getWorld().createEntity("planeOfDestruction");
    planeOfDestruction.setRotation(UnitQuat(Vec3(1.0f, 0.0f, 0.0f), toRadians(90.0f)));
    planeOfDestruction.setPosition(Vec3(0.0f, -25.0f, 0.0f));
    planeOfDestruction.addComponent<SoundComponent>(false, 5.0f, 1.0f);
    engine.getWorld().GetSystem<SoundSystem>()->registerEntity(planeOfDestruction);

    planeOfDestruction.addComponent<PhysicalComponent>(
        0.0f, PhysicalShape::CUBE, Vec3(200.5f, 200.01f, 0.0025f), 1.5f, 0.5f,
        [&](int id1, int id2) {
            engine.getWorld().getEntity(id2).playSound(rm.get<SoundBuffer>("sounds/ping"));
            auto &engine = Application::getInstance();
            auto &e = engine.getWorld().getEntity(id2);
            engine.getWorld().destroyEntity(e);
        });
    engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(planeOfDestruction);

    Entity &ship = engine.getWorld().createEntity("ship");
    ship.addComponent<VisualComponent>(rm.get<Mesh>("meshes/fighter2"),
                                       rm.get<Material>("materials/aluminium"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(ship);
    ship.addComponent<MovementComponent>();
    ship.getComponent<MovementComponent>().speed = 10.0f;
    engine.getWorld().attachEntities(ship, engine.getWorld().getEntity("fpsCamera"),
                                     PropagationType::POSITION_ROTATION);
    ship.addComponent<FireComponent>(1);
    engine.getWorld().GetSystem<FireSystem>()->registerEntity(ship);
    ship.addComponent<SoundComponent>(false, 0.25f);
    engine.getWorld().GetSystem<SoundSystem>()->registerEntity(ship);

    Entity &ball = engine.getWorld().createEntity("ball1");
    ball.setPosition(Vec3(2.0f, 0.0f, 0.0f));
    ball.addComponent<SoundComponent>(true);
    engine.getWorld().GetSystem<SoundSystem>()->registerEntity(ball);

    ball.addComponent<VisualComponent>(rm.get<Mesh>("meshes/sphere"),
                                       rm.get<Material>("materials/orange_plastic"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(ball);

    Entity &rotationAnchor = engine.getWorld().createEntity("rotation_anchor");
    rotationAnchor.setPosition(Vec3(0.0f, 0.5f, 0.0f));
    rotationAnchor.addComponent<RotationComponent>();
    rotationAnchor.getComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
    rotationAnchor.getComponent<RotationComponent>().speed = 2.0f;
    engine.getWorld().GetSystem<RotationSystem>()->registerEntity(rotationAnchor);

    engine.getWorld().attachEntities(rotationAnchor, ball, PropagationType::POSITION_ROTATION);
    rotationAnchor.addComponent<VisualComponent>(rm.get<Mesh>("meshes/cube"),
                                                 rm.get<Material>("materials/yellow_plastic"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(rotationAnchor);

    Entity &spaceStation = engine.getWorld().createEntity("space_station");
    spaceStation.setPosition(Vec3(200.0f, 100.0f, -300.0f));
    spaceStation.rotate(UnitQuat(Vec3(0.0f, 0.0f, 1.0f), toRadians(45.0f)));
    spaceStation.addComponent<RotationComponent>();
    spaceStation.getComponent<RotationComponent>().axis =
        Vec3(0.0f, 1.0f, 0.0f).rotate(UnitQuat(Vec3(0.0f, 0.0f, 1.0f), toRadians(45.0f)));
    spaceStation.getComponent<RotationComponent>().speed = 0.2f;
    engine.getWorld().GetSystem<RotationSystem>()->registerEntity(spaceStation);
    spaceStation.addComponent<VisualComponent>(rm.get<Mesh>("meshes/spacestation"),
                                               rm.get<Material>("materials/aluminium"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(spaceStation);

    Entity &planet = engine.getWorld().createEntity("planet");
    planet.setPosition(Vec3(-1.0f, 2.0f, 2.0));
    planet.addComponent<RotationComponent>();
    planet.getComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
    planet.getComponent<RotationComponent>().speed = .2f;
    engine.getWorld().GetSystem<RotationSystem>()->registerEntity(planet);
    planet.addComponent<VisualComponent>(rm.get<Mesh>("meshes/icosphere"),
                                         rm.get<Material>("materials/planet"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(planet);

    engine.startMainLoop();
    return 0;
}