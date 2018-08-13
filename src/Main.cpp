
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

#include "Nova.h"
//#include <string>
using namespace Nova;

int main()
{
    auto &engine = Application::getInstance();
    engine.startUp();
    // tests::testMatLibrary(); /*engine.shutDown();*/ return 0;
    // auto& fs = FileSystem::getInstance();
    auto &rm = ResourceManager::getInstance();

    bool skyboxBool = false;
    bool cursorShown = true;
    bool rotatePlanet = true;
    // bool changeMeshMaterial = false;
    bool changeCamera = false;
    bool light = true;

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
                Skybox skybox;
                skybox.skyboxTexture =
                    ResourceManager::getInstance().get<TextureCube>("city_night01_skybox");
                skybox.iblData = ResourceManager::getInstance().get<IBL_Data>("city_night01_IBL");
                GraphicsSystem::getInstance().getRendererBackend().setSkyBox(skybox);
            }
            else
            {
                Skybox skybox;
                skybox.skyboxTexture =
                    ResourceManager::getInstance().get<TextureCube>("fireSky_skybox");
                skybox.iblData = ResourceManager::getInstance().get<IBL_Data>("fireSky_IBL");
                GraphicsSystem::getInstance().getRendererBackend().setSkyBox(skybox);
            }
        }
        if (keyboard.getKeyState(Keys::KEY_2))
        {
            if (rotatePlanet)
            {
                rotatePlanet = false;
                Entity &planet = world.getEntity("star");
                world.GetSystem<RotationSystem>()->unregisterEntity(planet);
            }
            else
            {
                rotatePlanet = true;
                Entity &planet = world.getEntity("star");
                world.GetSystem<RotationSystem>()->registerEntity(planet);
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
                // engine.getWorld().GetSystem<PlayerInputSystem>()->unregisterEntity(
                //     world.getEntity("Camera1"));
                // world.getEntity("Camera1").setPosition(Vec3(0.0f, 0.0f, 2.0f));
                // world.getEntity("Camera1").setRotation(UnitQuat());
                // world.attachEntities(world.getEntity("rotation_anchor"),
                //                      world.getEntity("Camera1"));
                // world.getEntity("Camera1").setPosition(Vec3(0.0f, 0.0f, 2.0f));

                // engine.getWorld().GetSystem<CameraSystem>()->registerEntity(
                //     engine.getWorld().getEntity("rotatingCamera"));
                // engine.getWorld().GetSystem<CameraSystem>()->registerEntity(
                //     engine.getWorld().getEntity("fpsCamera"));
                GraphicsSystem::getInstance().setCurrentCamera(
                    &engine.getWorld().getEntity("Default Camera"));

                Window::getInstance().showCursor();
            }
            else
            {
                changeCamera = true;
                // engine.getWorld().GetSystem<CameraSystem>()->registerEntity(
                //     engine.getWorld().getEntity("fpsCamera"));
                // engine.getWorld().GetSystem<CameraSystem>()->registerEntity(
                //     engine.getWorld().getEntity("rotatingCamera"));

                // world.detachEntity(world.getEntity("Camera1"));
                // engine.getWorld().GetSystem<PlayerInputSystem>()->registerEntity(
                //     world.getEntity("Camera1"));

                GraphicsSystem::getInstance().setCurrentCamera(
                    &engine.getWorld().getEntity("fpsCamera"));
                Window::getInstance().hideCursor();
            }
        }
        if (keyboard.getKeyState(Keys::KEY_R))
        {
            if (world.getEntity("little moon").getName() != INVALID_ENTITY_STRING)
            {
                world.destroyEntity(world.getEntity("little moon"));
            }
        }
        if (keyboard.getKeyState(Keys::KEY_3))
        {
            /*if (changeMeshMaterial)
                    {
                            changeMeshMaterial = false;
                            world.getEntity("star").GetComponent<VisualComponent>().mesh =
               ResourceManager::getInstance().get<Mesh>("angel");
                            world.GetSystem<VisualSystem>()->unregisterEntity(world.getEntity("star"));
                            world.GetSystem<VisualSystem>()->registerEntity(world.getEntity("star"));
                    }
                    else {
                            changeMeshMaterial = true;
                            world.getEntity("star").GetComponent<VisualComponent>().mesh =
               ResourceManager::getInstance().get<Mesh>("bunny");
                            world.GetSystem<VisualSystem>()->unregisterEntity(world.getEntity("star"));
                            world.GetSystem<VisualSystem>()->registerEntity(world.getEntity("star"));
                    }*/
        }
        if (keyboard.getKeyState(Keys::KEY_5))
        {
            if (light)
            {
                world.GetSystem<LightSystem>()->unregisterEntity(world.getEntity("moon"));
                light = false;
            }
            else
            {
                world.GetSystem<LightSystem>()->registerEntity(world.getEntity("moon"));
                light = true;
            }
        }
    });

    Entity &fpsCamera = engine.getWorld().createEntity("fpsCamera");
    fpsCamera.setPosition(Vec3(0.0f, 0.0f, 2.0f));
    fpsCamera.addComponent<CameraComponent>(Mat4::makePerspectiveMatrix(
        toRadians(60.0f),
        static_cast<float>(EngineSettings::getInstance().getInteger("Video", "width")) /
            EngineSettings::getInstance().getInteger("Video", "height"),
        0.01f, 100.0f));
    engine.getWorld().GetSystem<CameraSystem>()->registerEntity(
        engine.getWorld().getEntity("fpsCamera"));
    fpsCamera.addComponent<MovementComponent>();
    fpsCamera.GetComponent<MovementComponent>().speed = 2.0f;
    engine.getWorld().GetSystem<PlayerInputSystem>()->registerEntity(fpsCamera);
    // GraphicsSystem::getInstance().setCurrentCamera(&fpsCamera);

    Entity &floor = engine.getWorld().createEntity("floor");
    floor.setRotation(UnitQuat(Vec3(1.0f, 0.0f, 0.0f), toRadians(90.0f)));
    floor.addComponent<VisualComponent>(rm.get<Mesh>("floor_mesh"),
                                        rm.get<Material>("PBR_plastic"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(floor);
    floor.setPosition(Vec3(0.0f, -1.0f, 0.0f));

    Entity &star = engine.getWorld().createEntity("star");
    star.setPosition(Vec3{0.0f, -1.0f, 0.0f});
    star.addComponent<VisualComponent>(rm.get<Mesh>("ball"), rm.get<Material>("PBR_aluminium"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(star);
    // star.addComponent<RotationComponent>();
    // star.GetComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
    // star.GetComponent<RotationComponent>().speed = 1.0f;
    // engine.getWorld().GetSystem<RotationSystem>()->registerEntity(star);

    Entity &rotationAnchor = engine.getWorld().createEntity("rotation_anchor");
    rotationAnchor.setPosition(Vec3(0.0f, -0.5f, 0.0f));
    rotationAnchor.addComponent<RotationComponent>();
    rotationAnchor.GetComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
    rotationAnchor.GetComponent<RotationComponent>().speed = 2.0f;
    engine.getWorld().GetSystem<RotationSystem>()->registerEntity(rotationAnchor);
    engine.getWorld().attachEntities(rotationAnchor, engine.getWorld().getEntity("Default Camera"),
                                     PropagationType::POSITION_ROTATION);

    // creates some random colored lights
    Random rnd;
    int count = 0;
    int x = 3, y = 3;
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            count++;
            Entity &anchor = engine.getWorld().createEntity("anchor" + std::to_string(count));
            // LOG_DEBUG(std::to_string(j + i*j));
            anchor.setPosition(Vec3((float)j - x / 2.0f, 0.0f, (float)i - y / 2.0f));
            anchor.addComponent<RotationComponent>();
            anchor.GetComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
            anchor.GetComponent<RotationComponent>().speed = (float)rnd.nextDouble(1.0, 10.0);
            engine.getWorld().GetSystem<RotationSystem>()->registerEntity(anchor);

            Entity &light = engine.getWorld().createEntity("light" + std::to_string(count));

            // anchor.addComponent<VisualComponent>(rm.get<Mesh>("box1"),
            //                                      rm.get<Material>("PBR_woodframe"));
            // engine.getWorld().GetSystem<VisualSystem>()->registerEntity(anchor);

            light.setPosition(Vec3(2.0f, 0.0f, 0.0f));
            light.setScale(Vec3(0.1f));
            light.addComponent<LightComponent>(LightType::POINT_LIGHT,
                                               Vec3{(float)rnd.nextDouble(0.1, 1.0),
                                                    (float)rnd.nextDouble(0.1, 1.0),
                                                    (float)rnd.nextDouble(0.1, 1.0)},
                                               false);
            // light.addComponent<LightComponent>(LightType::POINT_LIGHT, Vec3{ 1.0,1.0,1.0 },
            // false);
            engine.getWorld().GetSystem<LightSystem>()->registerEntity(light);

            engine.getWorld().attachEntities(anchor, light, PropagationType::POSITION_ROTATION);
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