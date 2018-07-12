#include "Nova.h"
// #include <iostream>
// #include "math/Vector.h"
// #include "math/Matrix.h"
// #include "math/Quaternion.h"
using namespace Nova;

int main() {			
	auto& engine = Application::getInstance();
	engine.startUp();
	//tests::testMatLibrary(); /*engine.shutDown();*/ return 0;
	auto& fs = FileSystem::getInstance();
	auto& rm = ResourceManager::getInstance();

	static bool skyboxBool = false;
	static bool cursorShown = false;
	static bool rotatePlanet = true;
	static bool changeMeshMaterial = false;
	static bool changeCamera = false;
	static bool changeProj = false;
	static bool light = true;
	
	engine.setKeyCallback([]()
	{
		auto& keyboard = InputSystem::getInstance().getKeyboard();
		auto& world = Application::getInstance().getWorld();
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
				if (skyboxBool) {
					Skybox skybox;
					skybox.skyboxTexture = ResourceManager::getInstance().get<TextureCube>("city_night01_skybox");
					skybox.iblData = ResourceManager::getInstance().get<IBL_Data>("city_night01_IBL");
					GraphicsSystem::getInstance().getRendererBackend().
						setSkyBox(skybox);					
				}
				else {
					Skybox skybox;
					skybox.skyboxTexture = ResourceManager::getInstance().get<TextureCube>("fireSky_skybox");
					skybox.iblData = ResourceManager::getInstance().get<IBL_Data>("fireSky_IBL");
					GraphicsSystem::getInstance().getRendererBackend().
						setSkyBox(skybox);
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
				else {
					rotatePlanet = true;
					Entity &planet = world.getEntity("star");
					world.GetSystem<RotationSystem>()->registerEntity(planet);
				}
			}
			if (keyboard.getKeyState(Keys::KEY_ESC))
			{
				if (cursorShown)
				{
					cursorShown = false;
					Window::getInstance().hideCursor();
					Entity &ship = world.getEntity("ship");
					world.GetSystem<PlayerInputSystem>()->registerEntity(ship);
				}
				else {
					cursorShown = true;
					Window::getInstance().showCursor();
					Entity &ship = world.getEntity("ship");
					world.GetSystem<PlayerInputSystem>()->unregisterEntity(ship);
				}
				//mIsClosing = true;
			}
			if (keyboard.getKeyState(Keys::KEY_F1))
			{
				if (changeCamera)
				{
					changeCamera = false;
					world.getEntity("Camera1").setPosition(Vec3(0.0f, 0.0f, 2.0f));
					world.attachEntities(world.getEntity("rotation_anchor"), world.getEntity("Camera1"));
				}
				else {
					changeCamera = true;
					world.getEntity("Camera1").setPosition(Vec3(0.0f, 0.0f, 1.0f));
					world.attachEntities(world.getEntity("ship"), world.getEntity("Camera1"));
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
					world.getEntity("star").GetComponent<VisualComponent>().mesh = ResourceManager::getInstance().get<Mesh>("angel");
					world.GetSystem<VisualSystem>()->unregisterEntity(world.getEntity("star"));
					world.GetSystem<VisualSystem>()->registerEntity(world.getEntity("star"));
				}
				else {
					changeMeshMaterial = true;
					world.getEntity("star").GetComponent<VisualComponent>().mesh = ResourceManager::getInstance().get<Mesh>("bunny");
					world.GetSystem<VisualSystem>()->unregisterEntity(world.getEntity("star"));
					world.GetSystem<VisualSystem>()->registerEntity(world.getEntity("star"));
				}*/
			}
			if (keyboard.getKeyState(Keys::KEY_4))
			{
				if (changeProj)
				{
					changeProj = false;
					world.getEntity("Camera1").GetComponent<CameraComponent>().projection =
						Mat4::makePerspectiveMatrix(toRadians(60.0f),
							static_cast<float>(EngineSettings::getInstance().getInteger("Video", "width")) /
							EngineSettings::getInstance().getInteger("Video", "height"),
							0.01f, 100.0f);
					world.GetSystem<CameraSystem>()->unregisterEntity(world.getEntity("Camera1"));
					world.GetSystem<CameraSystem>()->registerEntity(world.getEntity("Camera1"));
				}
				else {
					changeProj = true;
					float aspect = static_cast<float>(EngineSettings::getInstance().getInteger("Video", "width")) /
						EngineSettings::getInstance().getInteger("Video", "height");
					world.getEntity("Camera1").GetComponent<CameraComponent>().projection =
						Mat4::makeOrthographicMatrix(
							1.0f, -1.0f, 1.0f, -1.0f,
							0.01f, 100.0f);
					world.GetSystem<CameraSystem>()->unregisterEntity(world.getEntity("Camera1"));
					world.GetSystem<CameraSystem>()->registerEntity(world.getEntity("Camera1"));
				}
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
	
	Entity& camera = engine.getWorld().createEntity("Camera1");
	//camera.setPosition(Vec3(0.0f, 0.5f, 2.0f));	
	camera.setPosition(Vec3(0.0f, 0.0f, 2.0f));
	camera.addComponent<CameraComponent>(Mat4::makePerspectiveMatrix(toRadians(60.0f),
		static_cast<float>(EngineSettings::getInstance().getInteger("Video", "width")) /
		EngineSettings::getInstance().getInteger("Video", "height"),
		0.01f, 100.0f));
	engine.getWorld().GetSystem<CameraSystem>()->registerEntity(engine.getWorld().getEntity("Camera1"));
	/*camera.addComponent<MovementComponent>();	
	camera.GetComponent<MovementComponent>().speed = 2.0f;
	engine.getWorld().GetSystem<PlayerInputSystem>()->registerEntity(camera);*/
	/*float aspect = static_cast<float>(EngineSettings::getInstance().getInteger("Video", "width")) /
		EngineSettings::getInstance().getInteger("Video", "height");
	camera.addComponent<CameraComponent>(Mat4::makeOrthographicMatrix(
		1.0f*aspect, -1.0f*aspect, 1.0f, -1.0f,
		0.01f, 100.0f));*/
	
	//rm.get<Texture>("white_blue_checkerboard")->setFiltering(Filtering::NEAREST);
	Entity& floor = engine.getWorld().createEntity("floor");
	floor.setRotation(UnitQuat(Vec3(1.0f, 0.0f, 0.0f), toRadians(90.0f)));
	floor.addComponent<VisualComponent>(rm.get<Mesh>("floor_mesh"), rm.get<Material>("PBR_plastic"));	
	engine.getWorld().GetSystem<VisualSystem>()->registerEntity(floor);
	floor.setPosition(Vec3(0.0f,-1.0f,0.0f));

	/*Entity& northWall = engine.getWorld().createEntity("northWall");
	northWall.setRotation(UnitQuat(Vec3(0.0f, 1.0f, 0.0f), toRadians(0.0f)));
	northWall.addComponent<VisualComponent>(rm.get<Mesh>("floor_mesh"), rm.get<Material>("PBR_plastic"));
	engine.getWorld().GetSystem<VisualSystem>()->registerEntity(northWall);
	northWall.setPosition(Vec3(00.0f, 2.0f, 5.0f));

	Entity& westWall = engine.getWorld().createEntity("westWall");
	westWall.setRotation(UnitQuat(Vec3(0.0f, 1.0f, 0.0f), toRadians(90.0f)));
	westWall.addComponent<VisualComponent>(rm.get<Mesh>("floor_mesh"), rm.get<Material>("PBR_plastic"));
	engine.getWorld().GetSystem<VisualSystem>()->registerEntity(westWall);
	westWall.setPosition(Vec3(5.0f, 2.0f, 0.0f));*/

	//Entity& littltinyemoon = engine.getWorld().createEntity("little tiny moon");
	//littltinyemoon.setPosition(Vec3(0.25f, 0.0f, 0.0f));
	//littltinyemoon.addComponent<VisualComponent>(rm.get<Mesh>("box1"), rm.get<Material>("chekerboard_material"));
	//engine.getWorld().GetSystem<VisualSystem>()->registerEntity(littltinyemoon);
	//littltinyemoon.setScale(.0625f);

	//Entity& littlemoon = engine.getWorld().createEntity("little moon");
	//littlemoon.setPosition(Vec3(0.75f, 0.0f, 0.0f));
	//littlemoon.addComponent<VisualComponent>(rm.get<Mesh>("sphere1"), rm.get<Material>("planet_material"));
	//engine.getWorld().GetSystem<VisualSystem>()->registerEntity(littlemoon);
	//littlemoon.addComponent<RotationComponent>();
	//littlemoon.GetComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
	//littlemoon.GetComponent<RotationComponent>().speed = 0.2f;
	//engine.getWorld().GetSystem<RotationSystem>()->registerEntity(littlemoon);	
	//engine.getWorld().attachEntities(littlemoon, littltinyemoon, PropagationType::POSITION_ONLY);
	//littlemoon.setScale(.25f);
	//
	//Entity& moon = engine.getWorld().createEntity("moon");
	//moon.setPosition(Vec3(1.75f, 0.0f, 0.0f));
	////moon.addComponent<LightComponent>(LightType::POINT_LIGHT, Vec3{ 0.2f }, Vec3{ 1.0f,0.0f,0.0f }, Vec3{ 1.0f });
	////engine.getWorld().GetSystem<LightSystem>()->registerEntity(moon);
	//moon.addComponent<VisualComponent>(rm.get<Mesh>("sphere1"), rm.get<Material>("planet_material"));
	//engine.getWorld().GetSystem<VisualSystem>()->registerEntity(moon);
	//moon.addComponent<RotationComponent>();
	//moon.GetComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
	//moon.GetComponent<RotationComponent>().speed = 0.2f;
	//engine.getWorld().GetSystem<RotationSystem>()->registerEntity(moon);
	//engine.getWorld().attachEntities(moon, littlemoon, PropagationType::POSITION_ROTATION_SCALING);
	//moon.setScale(0.5f);

	Entity& star = engine.getWorld().createEntity("star");
	star.setPosition(Vec3{ 0.0f, -1.0f, 0.0f });	
	//star.addComponent<VisualComponent>(std::make_shared<Mesh>(Mesh::makeIcoSphere(1.0f, 3)), rm.get<Material>("PBR_aluminium"));
	star.addComponent<VisualComponent>(rm.get<Mesh>("dragon"), rm.get<Material>("PBR_aluminium"));
	engine.getWorld().GetSystem<VisualSystem>()->registerEntity(star);
	//star.addComponent<LightComponent>(LightType::POINT_LIGHT, Vec3{ 0.2f }, Vec3{ 1.0f,1.0f,1.0f }, Vec3{ 1.0f });
	//engine.getWorld().GetSystem<LightSystem>()->registerEntity(star);
	star.addComponent<RotationComponent>();
	star.GetComponent<RotationComponent>().axis = Vec3(0.0f,1.0f, 0.0f);
	star.GetComponent<RotationComponent>().speed = 1.0f;
	//engine.getWorld().GetSystem<RotationSystem>()->registerEntity(star);
	//star.addComponent<MovementComponent>();
	//star.GetComponent<MovementComponent>().forward = Vec3(0.0f, 0.0f, 1.0f);
	//star.GetComponent<MovementComponent>().speed = 0.04f;
	//engine.mWorld.GetSystem<PlayerInputSystem>()->registerEntity(star);	
	//engine.getWorld().attachEntities(star, moon, PropagationType::POSITION_ONLY);
	//star.rotate(Vec3{ 0.0f,0.0f,1.0f }.normalize(), toRadians(45.0f));

	Entity& ship = engine.getWorld().createEntity("ship");
	//ship.setRotation(UnitQuat(Vec3(1.0f, 0.0f, 0.0f), toRadians(-45.0f)));
	//ship.setPosition(Vec3(0.0f,-0.5f,1.0f));	
	ship.setPosition(Vec3(0.0f,0.0f,-1.0f));	
	//ship.addComponent<LightComponent>(Vec3{ 0.2f }, Vec3{ 0.0f,0.0f,1.0f }, Vec3{ 1.0f });
	//engine.getWorld().GetSystem<LightSystem>()->registerEntity(ship);
	//ship.addComponent<VisualComponent>(rm.get<Mesh>("spaceship"), rm.get<Material>("PBR_greasy_pan"));
	//engine.getWorld().GetSystem<VisualSystem>()->registerEntity(ship);
	ship.addComponent<MovementComponent>();
	//ship.GetComponent<MovementComponent>().forward = Vec3(0.0f, 0.0f, 1.0f);
	ship.GetComponent<MovementComponent>().speed = 2.0f;
	engine.getWorld().GetSystem<PlayerInputSystem>()->registerEntity(ship);
	//engine.getWorld().attachEntities(ship, camera, PropagationType::POSITION_ROTATION);


	//Entity& sunLightAnchor = engine.getWorld().createEntity("sun_light_anchor");
	//sunLightAnchor.addComponent<RotationComponent>();
	//sunLightAnchor.GetComponent<RotationComponent>().axis = Vec3(0.0f, 0.0f, 1.0f);
	//sunLightAnchor.GetComponent<RotationComponent>().speed = 0.1f;
	////engine.getWorld().GetSystem<RotationSystem>()->registerEntity(sunLightAnchor);

	Entity& rotationAnchor = engine.getWorld().createEntity("rotation_anchor");
	rotationAnchor.setPosition(Vec3(0.0f, -0.5f, 0.0f));
	rotationAnchor.addComponent<RotationComponent>();
	rotationAnchor.GetComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
	rotationAnchor.GetComponent<RotationComponent>().speed = 2.0f;
	engine.getWorld().GetSystem<RotationSystem>()->registerEntity(rotationAnchor);
	engine.getWorld().attachEntities(rotationAnchor, camera, PropagationType::POSITION_ROTATION);

	//Entity& sunLight = engine.getWorld().createEntity("sun_light");
	//sunLight.addComponent<VisualComponent>(rm.get<Mesh>("box1"), rm.get<Material>("face_material"));
	//engine.getWorld().GetSystem<VisualSystem>()->registerEntity(sunLight);
	//sunLight.setPosition(Vec3(5.0f, 5.0f, 0.0f));
	//sunLight.setScale(Vec3(0.2f));
	//sunLight.addComponent<LightComponent>(LightType::DIRECTIONAL_LIGHT, Vec3{ 1.0f }, true);
	////engine.getWorld().GetSystem<LightSystem>()->registerEntity(sunLight);

	//engine.getWorld().attachEntities(sunLightAnchor, sunLight);

	//Random rnd;
	//int count = 0;
	//int x = 3, y = 3;
	//for (int i = 0; i < x; i++)
	//{
	//	for (int j = 0; j < y; j++)
	//	{
	//		count++;
	//		Entity& anchor = engine.getWorld().createEntity("anchor" + std::to_string(count));
	//		//LOG_DEBUG(std::to_string(j + i*j));
	//		anchor.setPosition(Vec3((float)j-x/2.0f , 0.0f, (float)i-y/2.0f));
	//		anchor.addComponent<RotationComponent>();
	//		anchor.GetComponent<RotationComponent>().axis = Vec3(0.0f, 1.0f, 0.0f);
	//		anchor.GetComponent<RotationComponent>().speed = (float)rnd.nextDouble(1.0, 10.0);
	//		engine.getWorld().GetSystem<RotationSystem>()->registerEntity(anchor);

	//		Entity& light = engine.getWorld().createEntity("light" + std::to_string(count));
	//		//anchor.addComponent<VisualComponent>(rm.get<Mesh>("box1"), rm.get<Material>("PBR_woodframe"));
	//		//engine.getWorld().GetSystem<VisualSystem>()->registerEntity(anchor);
	//		light.setPosition(Vec3(2.0f, 0.0f, 0.0f));
	//		light.setScale(Vec3(0.1f));
	//		light.addComponent<LightComponent>(LightType::POINT_LIGHT,
	//			Vec3{ (float)rnd.nextDouble(0.1, 1.0),(float)rnd.nextDouble(0.1, 1.0),(float)rnd.nextDouble(0.1, 1.0) },
	//			false);
	//		//light.addComponent<LightComponent>(LightType::POINT_LIGHT, Vec3{ 1.0,1.0,1.0 }, false);
	//		engine.getWorld().GetSystem<LightSystem>()->registerEntity(light);

	//		engine.getWorld().attachEntities(anchor, light, PropagationType::POSITION_ROTATION);
	//	}
	//}
	
	engine.startMainLoop();
	//Window::getInstance().windowLoop();	
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
	//engine.shutDown();
	//Engine::shutDown();			
	return 0;
}