
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
#include <string>
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
    // auto& fs = FileSystem::getInstance();
    auto &rm = ResourceManager::getInstance();

    bool skyboxBool = false;
    bool cursorShown = true;
    bool rotatePlanet = true;
    // bool changeMeshMaterial = false;
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
                Skybox skybox;
                skybox.skyboxTexture =
                    ResourceManager::getInstance().get<TextureCube>("textures/skyboxes/city_night");
                skybox.iblData = ResourceManager::getInstance().get<IBL_Data>(
                    "textures/skyboxes/city_night_IBL");
                GraphicsSystem::getInstance().getRendererBackend().setSkyBox(skybox);
            }
            else
            {
                Skybox skybox;
                skybox.skyboxTexture =
                    ResourceManager::getInstance().get<TextureCube>("textures/skyboxes/fireSky");
                skybox.iblData =
                    ResourceManager::getInstance().get<IBL_Data>("textures/skyboxes/fireSky_IBL");
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
                engine.getWorld().GetSystem<CameraSystem>()->unregisterEntity(
                    engine.getWorld().getEntity("fpsCamera"));
                engine.getWorld().GetSystem<PlayerInputSystem>()->unregisterEntity(
                    engine.getWorld().getEntity("fpsCamera"));
                engine.getWorld().GetSystem<CameraSystem>()->registerEntity(
                    engine.getWorld().getEntity("Default Camera"));

                // engine.getWorld().GetSystem<PlayerInputSystem>()->unregisterEntity(
                //     engine.getWorld().getEntity("ship"));

                GraphicsSystem::getInstance().setCurrentCamera(
                    &engine.getWorld().getEntity("Default Camera"));

                Window::getInstance().showCursor();
            }
            else
            {
                changeCamera = true;
                engine.getWorld().GetSystem<CameraSystem>()->registerEntity(
                    engine.getWorld().getEntity("fpsCamera"));
                engine.getWorld().GetSystem<PlayerInputSystem>()->registerEntity(
                    engine.getWorld().getEntity("fpsCamera"));
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
            Entity &thing = engine.getWorld().createEntity("thing" + std::to_string(objCount));
            // LOG_DEBUG(std::to_string(j + i*j));
            thing.setPosition(Vec3(rnd.nextFloat(-5.0, 5.0), 2.0f, rnd.nextFloat(-5.0, 5.0)));
            thing.setRotation(UnitQuat(Vec3(1.0f, 0.0f, 0.0f), toRadians(45.0f)));
            thing.addComponent<RotationComponent>();
            thing.GetComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
            thing.GetComponent<RotationComponent>().speed = (float)rnd.nextDouble(1.0, 10.0);
            // engine.getWorld().GetSystem<RotationSystem>()->registerEntity(thing);
            if (rnd.nextDouble() > .5)
            {
                thing.setScale(1.0f);
                thing.addComponent<VisualComponent>(rm.get<Mesh>("meshes/cube"),
                                                    rm.get<Material>("materials/yellow_plastic"));
                thing.addComponent<PhysicalComponent>(1.0f, PhysicalShape::CUBE,
                                                      thing.getTransformStruct().scale / 4.0f,
                                                      thing.getTransformStruct().scale);
            }
            else
            {
                thing.setScale(0.5f);
                thing.addComponent<VisualComponent>(rm.get<Mesh>("meshes/sphere"),
                                                    rm.get<Material>("materials/worn_cement"));
                thing.addComponent<PhysicalComponent>(1.0f, PhysicalShape::SPHERE,
                                                      thing.getTransformStruct().scale,
                                                      thing.getTransformStruct().scale);
            }
            engine.getWorld().GetSystem<VisualSystem>()->registerEntity(thing);
            engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(thing);

            if (rnd.nextDouble() < .1)
            {
                thing.addComponent<LightComponent>(LightType::POINT_LIGHT,
                                                   Vec3{(float)rnd.nextDouble(0.1, 1.0),
                                                        (float)rnd.nextDouble(0.1, 1.0),
                                                        (float)rnd.nextDouble(0.1, 1.0)},
                                                   false);
                engine.getWorld().GetSystem<LightSystem>()->registerEntity(thing);
            }
        }
        if (keyboard.getKeyState(Keys::KEY_R))
        {
            // if (world.getEntity("thing" + std::to_string(objCount)).getName() !=
            //     INVALID_ENTITY_STRING)
            {
                world.destroyEntity(world.getEntity("thing" + std::to_string(objCount)));
                if (objCount > 0) objCount--;
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

    Entity &fpsCamera = engine.getWorld().createEntity("fpsCamera");
    fpsCamera.setPosition(Vec3(0.0f, 0.0f, -12.0f));
    fpsCamera.setRotation(UnitQuat(Vec3(0.0f, 1.0f, 0.0f), toRadians(180.0f)));
    fpsCamera.addComponent<CameraComponent>(Mat4::makePerspectiveMatrix(
        toRadians(60.0f),
        static_cast<float>(EngineSettings::getInstance().getInteger("Video", "width")) /
            EngineSettings::getInstance().getInteger("Video", "height"),
        0.01f, 100.0f));
    engine.getWorld().GetSystem<CameraSystem>()->registerEntity(
        engine.getWorld().getEntity("fpsCamera"));
    fpsCamera.addComponent<MovementComponent>();
    fpsCamera.GetComponent<MovementComponent>().speed = 2.0f;
    // engine.getWorld().GetSystem<PlayerInputSystem>()->registerEntity(fpsCamera);

    // GraphicsSystem::getInstance().setCurrentCamera(&fpsCamera);

    Entity &floor = engine.getWorld().createEntity("floor");
    floor.setRotation(UnitQuat(Vec3(1.0f, 0.0f, 0.0f), toRadians(70.0f)));
    floor.addComponent<VisualComponent>(rm.get<Mesh>("meshes/floor"),
                                        rm.get<Material>("materials/orange_plastic"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(floor);
    floor.setPosition(Vec3(0.0f, 0.0f, 0.0f));
    floor.addComponent<PhysicalComponent>(0.0f, PhysicalShape::CUBE, Vec3(5.0f, 5.0f, 0.001f),
                                          floor.getTransformStruct().scale);
    engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(floor);

    Entity &star = engine.getWorld().createEntity("star");
    star.setPosition(Vec3{0.0f, 2.0f, 0.0f});
    star.addComponent<VisualComponent>(rm.get<Mesh>("meshes/sphere"),
                                       rm.get<Material>("materials/worn_cement"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(star);
    star.addComponent<PhysicalComponent>(1.0f, PhysicalShape::SPHERE, Vec3(0.5f),
                                         star.getTransformStruct().scale);
    engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(star);

    Entity &ship = engine.getWorld().createEntity("ship");
    ship.setPosition(Vec3{-1.0f, 0.0f, -2.0f});
    ship.setScale(0.25f);
    ship.addComponent<VisualComponent>(rm.get<Mesh>("meshes/cube"),
                                       rm.get<Material>("materials/yellow_plastic"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(ship);
    ship.addComponent<MovementComponent>();
    ship.GetComponent<MovementComponent>().speed = 2.0f;
    // ship.addComponent<PhysicalComponent>(1.0f, PhysicalShape::CUBE, Vec3(0.25f),
    // ship.getTransformStruct().scale);
    // engine.getWorld().GetSystem<PhysicalSystem>()->registerEntity(ship);

    // engine.getWorld().GetSystem<PlayerInputSystem>()->registerEntity(ship);
    Entity &ship2 = engine.getWorld().createEntity("ship2");
    engine.getWorld().attachEntities(ship, engine.getWorld().getEntity("ship2"),
                                     PropagationType::POSITION_ROTATION_SCALING);

    ship2.setPosition(Vec3{1.0f, 0.0f, 2.0f});
    // ship2.setScale(0.25f);
    ship2.addComponent<VisualComponent>(rm.get<Mesh>("meshes/cube"),
                                        rm.get<Material>("materials/worn_cement"));
    engine.getWorld().GetSystem<VisualSystem>()->registerEntity(ship2);
    ship2.addComponent<MovementComponent>();
    ship2.GetComponent<MovementComponent>().speed = 2.0f;
    // engine.getWorld().GetSystem<PlayerInputSystem>()->registerEntity(ship2);
    // engine.getWorld().attachEntities(ship2, engine.getWorld().getEntity("fpsCamera"),
    //                                  PropagationType::POSITION_ROTATION_SCALING);

    // star.addComponent<RotationComponent>();
    // star.GetComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
    // star.GetComponent<RotationComponent>().speed = 1.0f;
    // engine.getWorld().GetSystem<RotationSystem>()->registerEntity(star);

    Entity &rotationAnchor = engine.getWorld().createEntity("rotation_anchor");
    rotationAnchor.setPosition(Vec3(0.0f, 1.0f, 0.0f));
    rotationAnchor.addComponent<RotationComponent>();
    rotationAnchor.GetComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
    rotationAnchor.GetComponent<RotationComponent>().speed = 2.0f;
    engine.getWorld().GetSystem<RotationSystem>()->registerEntity(rotationAnchor);
    engine.getWorld().attachEntities(rotationAnchor, engine.getWorld().getEntity("Default Camera"),
                                     PropagationType::POSITION_ROTATION);

    // creates some random colored lights
    Random rnd;
    int count = 0;
    int x = 0, y = 0;
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
            anchor.addComponent<VisualComponent>(rm.get<Mesh>("meshes/cube"),
                                                 rm.get<Material>("materials/aluminium"));
            engine.getWorld().GetSystem<VisualSystem>()->registerEntity(anchor);

            Entity &light = engine.getWorld().createEntity("light" + std::to_string(count));
            light.setPosition(Vec3(2.0f, 0.0f, 0.0f));
            light.setScale(Vec3(0.1f));
            light.addComponent<VisualComponent>(rm.get<Mesh>("meshes/cube"),
                                                rm.get<Material>("materials/yellow_plastic"));
            light.addComponent<LightComponent>(LightType::POINT_LIGHT,
                                               Vec3{(float)rnd.nextDouble(0.1, 1.0),
                                                    (float)rnd.nextDouble(0.1, 1.0),
                                                    (float)rnd.nextDouble(0.1, 1.0)},
                                               false);
            engine.getWorld().GetSystem<LightSystem>()->registerEntity(light);
            engine.getWorld().GetSystem<VisualSystem>()->registerEntity(light);

            engine.getWorld().attachEntities(anchor, light,
                                             PropagationType::POSITION_ROTATION_SCALING);
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

// /*
// Bullet Continuous Collision Detection and Physics Library
// Copyright (c) 2003-2007 Erwin Coumans  http://continuousphysics.com/Bullet/

// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this
// software. Permission is granted to anyone to use this software for any purpose, including
// commercial applications, and to alter it and redistribute it freely, subject to the following
// restrictions:

// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the
// original software. If you use this software in a product, an acknowledgment in the product
// documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as
// being the original software.
// 3. This notice may not be removed or altered from any source distribution.
// */

// ///-----includes_start-----
// #include "btBulletDynamicsCommon.h"
// #include <stdio.h>

// /// This is a Hello World program for running a basic Bullet physics simulation

// int main(int argc, char **argv)
// {
//     ///-----includes_end-----

//     int i;
//     ///-----initialization_start-----

//     /// collision configuration contains default setup for memory, collision setup. Advanced
//     users
//     /// can create their own configuration.
//     btDefaultCollisionConfiguration *collisionConfiguration = new
//     btDefaultCollisionConfiguration();

//     /// use the default collision dispatcher. For parallel processing you can use a diffent
//     /// dispatcher (see Extras/BulletMultiThreaded)
//     btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);

//     /// btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
//     btBroadphaseInterface *overlappingPairCache = new btDbvtBroadphase();

//     /// the default constraint solver. For parallel processing you can use a different solver
//     (see
//     /// Extras/BulletMultiThreaded)
//     btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver;

//     btDiscreteDynamicsWorld *dynamicsWorld = new btDiscreteDynamicsWorld(
//         dispatcher, overlappingPairCache, solver, collisionConfiguration);

//     dynamicsWorld->setGravity(btVector3(0, -10, 0));

//     ///-----initialization_end-----

//     // keep track of the shapes, we release memory at exit.
//     // make sure to re-use collision shapes among rigid bodies whenever possible!
//     btAlignedObjectArray<btCollisionShape *> collisionShapes;

//     /// create a few basic rigid bodies

//     // the ground is a cube of side 100 at position y = -56.
//     // the sphere will hit it at y = -6, with center at -5
//     {
//         btCollisionShape *groundShape =
//             new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

//         collisionShapes.push_back(groundShape);
//         btTransform groundTransform;
//         groundTransform.setIdentity();
//         groundTransform.setOrigin(btVector3(0, -56, 0));

//         btScalar mass(0.);

//         // rigidbody is dynamic if and only if mass is non zero, otherwise static
//         bool isDynamic = (mass != 0.f);

//         btVector3 localInertia(0, 0, 0);
//         if (isDynamic) groundShape->calculateLocalInertia(mass, localInertia);

//         // using motionstate is optional, it provides interpolation capabilities, and only
//         // synchronizes 'active' objects
//         btDefaultMotionState *myMotionState = new btDefaultMotionState(groundTransform);
//         btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape,
//                                                         localInertia);
//         btRigidBody *body = new btRigidBody(rbInfo);

//         // add the body to the dynamics world
//         dynamicsWorld->addRigidBody(body);
//     }

//     {
//         // create a dynamic rigidbody

//         // btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
//         btCollisionShape *colShape = new btSphereShape(btScalar(1.));
//         collisionShapes.push_back(colShape);

//         /// Create Dynamic Objects
//         btTransform startTransform;
//         startTransform.setIdentity();

//         btScalar mass(1.f);

//         // rigidbody is dynamic if and only if mass is non zero, otherwise static
//         bool isDynamic = (mass != 0.f);

//         btVector3 localInertia(0, 0, 0);
//         if (isDynamic) colShape->calculateLocalInertia(mass, localInertia);

//         startTransform.setOrigin(btVector3(2, 10, 0));

//         // using motionstate is recommended, it provides interpolation capabilities, and only
//         // synchronizes 'active' objects
//         btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);
//         btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape,
//                                                         localInertia);
//         btRigidBody *body = new btRigidBody(rbInfo);

//         dynamicsWorld->addRigidBody(body);
//     }

//     /// Do some simulation

//     ///-----stepsimulation_start-----
//     for (i = 0; i < 150; i++)
//     {
//         dynamicsWorld->stepSimulation(1.f / 60.f, 10);

//         // print positions of all objects
//         for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
//         {
//             btCollisionObject *obj = dynamicsWorld->getCollisionObjectArray()[j];
//             btRigidBody *body = btRigidBody::upcast(obj);
//             btTransform trans;
//             if (body && body->getMotionState())
//             {
//                 body->getMotionState()->getWorldTransform(trans);
//             }
//             else
//             {
//                 trans = obj->getWorldTransform();
//             }
//             printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()),
//                    float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
//         }
//     }

//     ///-----stepsimulation_end-----

//     // cleanup in the reverse order of creation/initialization

//     ///-----cleanup_start-----

//     // remove the rigidbodies from the dynamics world and delete them
//     for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
//     {
//         btCollisionObject *obj = dynamicsWorld->getCollisionObjectArray()[i];
//         btRigidBody *body = btRigidBody::upcast(obj);
//         if (body && body->getMotionState())
//         {
//             delete body->getMotionState();
//         }
//         dynamicsWorld->removeCollisionObject(obj);
//         delete obj;
//     }

//     // delete collision shapes
//     for (int j = 0; j < collisionShapes.size(); j++)
//     {
//         btCollisionShape *shape = collisionShapes[j];
//         collisionShapes[j] = 0;
//         delete shape;
//     }

//     // delete dynamics world
//     delete dynamicsWorld;

//     // delete solver
//     delete solver;

//     // delete broadphase
//     delete overlappingPairCache;

//     // delete dispatcher
//     delete dispatcher;

//     delete collisionConfiguration;

//     // next line is optional: it will be cleared by the destructor when the array goes out of
//     scope collisionShapes.clear();
// }