#include "Nova.h"
#include <string>
//#include "graphics/opengl/TextureCube.h"

using namespace Nova;

int main()
{
    auto &engine = Application::getInstance();
    engine.startUp();
    {
        // tests::testMatLibrary();
        // /*engine.shutDown();*/
        // int a;
        // std::cin >> a;
        // return 0;
    }
    auto &rm = ResourceManager::getInstance();

    bool skyboxBool = false;
    bool cursorShown = true;
    bool rotatePlanet = true;
    // bool changeMeshMaterial = false;
    bool changeCamera = false;
    bool light = true;
    int objCount = 0;

    // rm.get<Mesh>("meshes/bunny");
    // rm.get<SoundBuffer>("sounds/ping");
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
                // Entity &planet = world.getEntity("star");
                // world.GetSystem<RotationSystem>()->unregisterEntity(planet);
                Entity &floor = world.getEntity("floor");
                floor.stopSound();
            }
            else
            {
                rotatePlanet = true;
                // Entity &planet = world.getEntity("star");
                // world.GetSystem<RotationSystem>()->registerEntity(planet);
                // Entity &floor = world.getEntity("floor");
                // floor.playSound(rm.get<SoundBuffer>("sounds/music1"));
            }
        }
        if (keyboard.getKeyState(Keys::KEY_ESC))
        {
            if (cursorShown)
            {
                // cursorShown = false;
                // Window::getInstance().hideCursor();
                // Entity &ship = world.getEntity("ship");
                // world.GetSystem<PlayerInputSystem>()->registerEntity(ship);
            }
            else
            {
                // cursorShown = true;
                // Window::getInstance().showCursor();
                // Entity &ship = world.getEntity("ship");
                // world.GetSystem<PlayerInputSystem>()->unregisterEntity(ship);
            }
            // mIsClosing = true;
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

                // engine.getWorld().GetSystem<PlayerInputSystem>()->unregisterEntity(
                //     engine.getWorld().getEntity("ship"));

                // GraphicsSystem::getInstance().setCurrentCamera(
                //     &engine.getWorld().getEntity("Default Camera"));

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

                // engine.getWorld().GetSystem<PlayerInputSystem>()->registerEntity(
                //     engine.getWorld().getEntity("ship"));

                GraphicsSystem::getInstance().setCurrentCamera(
                    &engine.getWorld().getEntity("fpsCamera"));
                Window::getInstance().hideCursor();
            }
        }
        if (keyboard.getKeyState(Keys::KEY_F))
        {
            Random rnd;
            objCount++;
            Entity &thing =
                engine.getWorld().createEntity("thing" + std::to_string(objCount));
            // LOG_DEBUG(std::to_string(j + i*j));
            //   thing.setPosition(
            //       Vec3(rnd.nextFloat(-5.0, 5.0), 5.0f, rnd.nextFloat(-5.0, 5.0)));

            //auto &cam = GraphicsSystem::getInstance().getCurrentCamera();
            auto &camEnti = engine.getWorld().getEntity("ship");
            thing.setPosition(camEnti.getTransformStruct().finalTranslation);
            // thing.setRotation(UnitQuat(Vec3(1.0f, 0.0f, 0.0f), toRadians(45.0f)));
            // thing.addComponent<RotationComponent>();
            // thing.getComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
            // thing.getComponent<RotationComponent>().speed =
            // (float)rnd.nextDouble(1.0, 10.0);
            // engine.getWorld().GetSystem<RotationSystem>()->registerEntity(thing);
            thing.addComponent<SoundComponent>(false, 5.0f, 1.0f);
            engine.getWorld().GetSystem<SoundSystem>()->registerEntity(thing);
            if (rnd.nextDouble() > .5)
            {
                thing.setScale(1.0f);
                thing.addComponent<VisualComponent>(
                    rm.get<Mesh>("meshes/cube"),
                    rm.get<Material>("materials/yellow_plastic"));
                thing.addComponent<PhysicalComponent>(
                    1.0f, PhysicalShape::CUBE, thing.getTransformStruct().scale / 4.0f,
                    1.5f, 0.5f);
            }
            else
            {
                thing.setScale(0.5f);
                thing.addComponent<VisualComponent>(
                    rm.get<Mesh>("meshes/dragon"),
                    rm.get<Material>("materials/aluminium"));
                thing.addComponent<PhysicalComponent>(1.0f, PhysicalShape::CUBE,
                                                      Vec3(.5f, .7f, .25f), 1.5f, 0.5f);
                // thing.getTransformStruct().scale
            }
            engine.getWorld().GetSystem<VisualSystem>()->registerEntity(thing);
            engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(thing);

            Physics::getInstance().pushObject(
                thing.getID(), camEnti.getTransformStruct().forward * 5.0f);

            //if (rnd.nextDouble() < .1)
            if (true)
            {
                thing.addComponent<LightComponent>(
                    LightType::POINT_LIGHT,
                    // Vec3{(float)rnd.nextDouble(0.1, 1.0),
                    //      (float)rnd.nextDouble(0.1, 1.0),
                    //      (float)rnd.nextDouble(0.1, 1.0)},
                    Vec3(1000.0),
                    false);
                engine.getWorld().GetSystem<LightSystem>()->registerEntity(thing);
            }
        }
        if (keyboard.getKeyState(Keys::KEY_R))
        {
            // if (world.getEntity("thing" + std::to_string(objCount)).getName() !=
            //     INVALID_ENTITY_STRING)
            {
                world.destroyEntity(
                    world.getEntity("thing" + std::to_string(objCount)));
                if (objCount > 0)
                    objCount--;
            }
        }
        if (keyboard.getKeyState(Keys::KEY_3))
        {
            /*if (changeMeshMaterial)
{
changeMeshMaterial = false;
world.getEntity("star").getComponent<VisualComponent>().mesh
= ResourceManager::getInstance().get<Mesh>("angel");
world.GetSystem<VisualSystem>()->unregisterEntity(world.getEntity("star"));
world.GetSystem<VisualSystem>()->registerEntity(world.getEntity("star"));
}
else {
changeMeshMaterial = true;
world.getEntity("star").getComponent<VisualComponent>().mesh
= ResourceManager::getInstance().get<Mesh>("bunny");
world.GetSystem<VisualSystem>()->unregisterEntity(world.getEntity("star"));
world.GetSystem<VisualSystem>()->registerEntity(world.getEntity("star"));
}*/
        }
        if (keyboard.getKeyState(Keys::KEY_5))
        {
            if (light)
            {
                world.GetSystem<LightSystem>()->unregisterEntity(
                    world.getEntity("light1"));
                light = false;
            }
            else
            {
                world.GetSystem<LightSystem>()->registerEntity(
                    world.getEntity("light1"));
                light = true;
            }
        }
    });

    Entity &fpsCamera = engine.getWorld().createEntity("fpsCamera");
    //fpsCamera.setPosition(Vec3(0.0f, 0.0f, -12.0f));
    fpsCamera.setPosition(Vec3(0.0f, 0.5f, 2.5f));
    //fpsCamera.setRotation(UnitQuat(Vec3(0.0f, 1.0f, 0.0f), toRadians(180.0f)));
    fpsCamera.addComponent<CameraComponent>(Mat4::makePerspectiveMatrix(
        toRadians(60.0f),
        static_cast<float>(
            EngineSettings::getInstance().getInteger("Video", "width")) /
            EngineSettings::getInstance().getInteger("Video", "height"),
        0.5f, 800.0f));
    engine.getWorld().GetSystem<CameraSystem>()->registerEntity(
        engine.getWorld().getEntity("fpsCamera"));
    fpsCamera.addComponent<MovementComponent>();
    fpsCamera.getComponent<MovementComponent>().speed = 10.0f;
    // engine.getWorld().GetSystem<PlayerInputSystem>()->registerEntity(fpsCamera);

    // GraphicsSystem::getInstance().setCurrentCamera(&fpsCamera);

    Entity &floor = engine.getWorld().createEntity("floor");
    floor.setRotation(UnitQuat(Vec3(1.0f, 0.0f, 0.0f), toRadians(90.0f)));
    floor.addComponent<VisualComponent>(
        rm.get<Mesh>("meshes/floor"),
        rm.get<Material>("materials/orange_plastic"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(floor);
    floor.setPosition(Vec3(0.0f, 0.0f, 0.0f));
    floor.addComponent<PhysicalComponent>(0.0f, PhysicalShape::CUBE,
                                          Vec3(5.0f, 5.0f, 0.001f), 1.5f, 0.5f);
    engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(floor);

    // floor.addComponent<SoundComponent>(false, 1.0f, 1.0f);
    // engine.getWorld().GetSystem<SoundSystem>()->registerEntity(floor);
    // // floor.playSound(rm.get<SoundBuffer>("sounds/music1"));

    // // Entity &star = engine.getWorld().createEntity("star");
    // // star.setPosition(Vec3{0.0f, 2.0f, 0.0f});
    // // star.addComponent<VisualComponent>(rm.get<Mesh>("meshes/bunny"),
    // //                                    rm.get<Material>("materials/worn_cement"));
    // // engine.getWorld().GetSystem<VisualSystem>()->registerEntity(star);
    // // star.addComponent<PhysicalComponent>(5.0f, PhysicalShape::CUBE, Vec3(.5f,
    // // .5f, .25f), 1.5f,
    // //                                      0.5f);
    // // engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(star);

    Entity &planeOfDestruction =
        engine.getWorld().createEntity("planeOfDestruction");
    planeOfDestruction.setRotation(
        UnitQuat(Vec3(1.0f, 0.0f, 0.0f), toRadians(90.0f)));
    planeOfDestruction.setPosition(Vec3(0.0f, -25.0f, 0.0f));
    // planeOfDestruction.addComponent<VisualComponent>(rm.get<Mesh>("meshes/floor"),
    // rm.get<Material>("materials/worn_cement"));
    // engine.getWorld().GetSystem<VisualSystem>()->registerEntity(planeOfDestruction);
    planeOfDestruction.addComponent<SoundComponent>(false, 5.0f, 1.0f);
    engine.getWorld().GetSystem<SoundSystem>()->registerEntity(
        planeOfDestruction);
    // Audio::getInstance().assignListener(planeOfDestruction);

    planeOfDestruction.addComponent<PhysicalComponent>(
        0.0f, PhysicalShape::CUBE, Vec3(200.5f, 200.01f, 0.0025f), 1.5f, 0.5f,
        [&](int id1, int id2) {
            engine.getWorld().getEntity(id2).playSound(
                rm.get<SoundBuffer>("sounds/ping"));
            auto &engine = Application::getInstance();
            auto &e = engine.getWorld().getEntity(id2);
            engine.getWorld().destroyEntity(e);
        });
    engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(
        planeOfDestruction);

    Entity &ship = engine.getWorld().createEntity("ship");
    //ship.setPosition(Vec3{-1.0f, 0.0f, -2.0f});
    //ship.setPosition(Vec3{-1.0f, 0.0f, -2.0f});
    //ship.setScale(0.25f);
    //ship.rotate(UnitQuat(Vec3(0.0f,1.0f,0.0f), toRadians(45.0f)));
    ship.addComponent<VisualComponent>(rm.get<Mesh>("meshes/fighter2"),
                                       rm.get<Material>("materials/aluminium"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(ship);
    ship.addComponent<MovementComponent>();
    ship.getComponent<MovementComponent>().speed = 10.0f;
    engine.getWorld().attachEntities(ship,
                                     engine.getWorld().getEntity("fpsCamera"),
                                     PropagationType::POSITION_ROTATION);

    // // ship.addComponent<PhysicalComponent>(1.0f, PhysicalShape::CUBE,
    // Vec3(0.25f),
    // // 1.5f, 0.5f);
    // // engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(ship);

    // // engine.getWorld().GetSystem<PlayerInputSystem>()->registerEntity(ship);
    // Entity &ship2 = engine.getWorld().createEntity("ship2");
    // engine.getWorld().attachEntities(ship,
    // engine.getWorld().getEntity("ship2"),
    //                                  PropagationType::POSITION_ROTATION_SCALING);

    // ship2.setPosition(Vec3{1.0f, 0.0f, 2.0f});
    // // ship2.setScale(0.25f);
    // ship2.addComponent<VisualComponent>(rm.get<Mesh>("meshes/cube"),
    //                                     rm.get<Material>("materials/worn_cement"));
    // engine.getWorld().GetSystem<VisualSystem>()->registerEntity(ship2);
    // ship2.addComponent<MovementComponent>();
    // ship2.getComponent<MovementComponent>().speed = 2.0f;
    // // engine.getWorld().GetSystem<PlayerInputSystem>()->registerEntity(ship2);
    // // engine.getWorld().attachEntities(ship2,
    // engine.getWorld().getEntity("fpsCamera"),
    // // PropagationType::POSITION_ROTATION_SCALING);

    // // star.addComponent<RotationComponent>();
    // // star.getComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
    // // star.getComponent<RotationComponent>().speed = 1.0f;
    // // engine.getWorld().GetSystem<RotationSystem>()->registerEntity(star);

    Entity &ball = engine.getWorld().createEntity("ball1");
    ball.setPosition(Vec3(2.0f, 0.0f, 0.0f));
    // auto &dc = engine.getWorld().getEntity("Default Camera");
    // ball.addComponent<PhysicalComponent>(-1.0f, PhysicalShape::SPHERE, Vec3(0.5f),
    //                                      1.5f, 0.5f);
    //engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(ball);

    ball.addComponent<SoundComponent>(true);
    // dc.playSound(rm.get<SoundBuffer>("sounds/ping"));
    engine.getWorld().GetSystem<SoundSystem>()->registerEntity(ball);

    ball.addComponent<VisualComponent>(
        rm.get<Mesh>("meshes/sphere"),
        rm.get<Material>("materials/orange_plastic"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(ball);

    Entity &rotationAnchor = engine.getWorld().createEntity("rotation_anchor");
    rotationAnchor.setPosition(Vec3(0.0f, 0.5f, 0.0f));
    rotationAnchor.addComponent<RotationComponent>();
    rotationAnchor.getComponent<RotationComponent>().axis =
        Vec3(0.0f, 1.0f, 0.0f);
    rotationAnchor.getComponent<RotationComponent>().speed = 2.0f;
    engine.getWorld().GetSystem<RotationSystem>()->registerEntity(rotationAnchor);

    //   rotationAnchor.addComponent<PhysicalComponent>(
    //       -1.0f, PhysicalShape::CUBE,
    //       rotationAnchor.getTransformStruct().scale / 4.0f, 1.5f, 0.5f);
    //engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(rotationAnchor);

    engine.getWorld().attachEntities(rotationAnchor, ball, PropagationType::POSITION_ROTATION);
    rotationAnchor.addComponent<VisualComponent>(
        rm.get<Mesh>("meshes/cube"),
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
    spaceStation.addComponent<VisualComponent>(
        rm.get<Mesh>("meshes/spacestation"),
        rm.get<Material>("materials/aluminium"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(spaceStation);

    // auto &rb = GraphicsSystem::getInstance().getRendererBackend();
    // auto tc = std::make_shared<TextureCube>(std::vector<void *>(), 1024, 1024);
    // rb.drawToTextureCube(ResourceManager::getInstance().get<GPUProgram>("shaders/noise_cube"), tc);
    // auto textures = std::vector<std::pair<std::shared_ptr<ITexture>, std::string>>{{tc, "uAlbedoMap"}};
    //auto noiseMaterial = std::make_shared<Material>(ResourceManager::getInstance().get<GPUProgram>("shaders/geometry_pass_PBR_cube"), textures);
    
    Entity &planet = engine.getWorld().createEntity("planet");
    planet.setPosition(Vec3(-1.0f, 2.0f, 2.0));
    planet.addComponent<RotationComponent>();
    planet.getComponent<RotationComponent>().axis =
        Vec3(0.0f, 1.0f, 0.0f);
    planet.getComponent<RotationComponent>().speed = .2f;
    engine.getWorld().GetSystem<RotationSystem>()->registerEntity(planet);
    planet.addComponent<VisualComponent>(
        rm.get<Mesh>("meshes/sphere"),
        rm.get<Material>("materials/planet"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(planet);

    // Entity &star = engine.getWorld().createEntity("star");
    // star.setPosition(Vec3(0.0f, 2.0f, 0.0));
    // star.rotate(UnitQuat(Vec3(0.0f, 1.0f, 0.0f), toRadians(180.0f)));
    // // star.addComponent<RotationComponent>();
    // // star.getComponent<RotationComponent>().axis =
    // //     Vec3(0.0f, 1.0f, 0.0f);
    // // star.getComponent<RotationComponent>().speed = .2f;
    // // engine.getWorld().GetSystem<RotationSystem>()->registerEntity(star);
    // star.addComponent<VisualComponent>(
    //     rm.get<Mesh>("meshes/quad"),
    //     rm.get<Material>("materials/billboard"));
    // engine.getWorld().GetSystem<VisualSystem>()->registerEntity(star);

    // creates some random colored lights
    Random rnd;
    int count = 0;
    int x = 0, y = 0;
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            count++;
            Entity &anchor =
                engine.getWorld().createEntity("anchor" + std::to_string(count));
            // LOG_DEBUG(std::to_string(j + i*j));
            anchor.setPosition(Vec3((float)j - x / 2.0f, 0.0f, (float)i - y / 2.0f));
            anchor.addComponent<RotationComponent>();
            anchor.getComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
            anchor.getComponent<RotationComponent>().speed =
                (float)rnd.nextDouble(1.0, 10.0);
            engine.getWorld().GetSystem<RotationSystem>()->registerEntity(anchor);
            anchor.addComponent<VisualComponent>(
                rm.get<Mesh>("meshes/cube"), rm.get<Material>("materials/aluminium"));
            engine.getWorld().GetSystem<VisualSystem>()->registerEntity(anchor);

            Entity &light =
                engine.getWorld().createEntity("light" + std::to_string(count));
            light.setPosition(Vec3(2.0f, 0.0f, 0.0f));
            light.setScale(Vec3(0.1f));
            light.addComponent<VisualComponent>(
                rm.get<Mesh>("meshes/cube"),
                rm.get<Material>("materials/yellow_plastic"));
            light.addComponent<LightComponent>(LightType::POINT_LIGHT,
                                               Vec3{(float)rnd.nextDouble(0.1, 1.0),
                                                    (float)rnd.nextDouble(0.1, 1.0),
                                                    (float)rnd.nextDouble(0.1, 1.0)},
                                               false);
            engine.getWorld().GetSystem<LightSystem>()->registerEntity(light);
            engine.getWorld().GetSystem<VisualSystem>()->registerEntity(light);

            engine.getWorld().attachEntities(
                anchor, light, PropagationType::POSITION_ROTATION_SCALING);
        }
    }

    engine.startMainLoop();
    /*float out = 0;
StringToInt("42") >> IntSquare() >> DivideBy42F() >> out;
LOG_INFO(out);*/

    /*std::vector<int> a{ 1,456,1,2,3,7,5,456,7,68,3,2,4 };
std::vector<int> b;
SortRenderPackets(a) >> b;
std::string s;
for (auto v : b)
s = s + " " + std::to_string(v);
LOG_INFO(s);*/
    // engine.shutDown();
    return 0;
}
