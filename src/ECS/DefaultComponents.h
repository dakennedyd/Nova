#pragma once
#include <memory>
#include "graphics/opengl/RendererInit.h"
#include "graphics/opengl/FrameBuffer.h"
#include "Component.h"
#include "math/Vector.h"
#include "math/Matrix.h"

namespace Nova {
	class Mesh;
	class Material;
	/*struct TransformComponent final :public IComponent
	{
	Vec3 mScale;
	UnitQuat mRotation;
	Vec3 mTranslation;

	Mat4 mTransform;
	Mat4 mNormal;
	};*/
	struct VisualComponent final :public IComponent
	{
		VisualComponent() = delete;
		VisualComponent(const std::shared_ptr<Mesh> &mesh, const  std::shared_ptr<Material> &material)
			:mesh(mesh), material(material) {};
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Material> material;
	};

	struct RotationComponent final :public IComponent
	{
		float speed = 0; //in RPM
		Vec3 axis;
	};

	struct MovementComponent final :public IComponent
	{
		float speed = 1.0f;
	};

	struct CameraComponent final :public IComponent
	{
		CameraComponent() = delete;
		CameraComponent(const Mat4 & projMatrix)
			:projection(projMatrix) {};
		Mat4 projection;
		Mat4 view;
		//Vec3 position;
		//Vec3 forwardVector;
	};

	struct LightComponent final :public IComponent
	{
		LightComponent() = delete;
		LightComponent(const LightType &lightType, const Vec3& color, const bool castsShadows = false)
			:type(lightType), color(color)//, diffuseColor(diff), specularColor(spec)
		{}
		std::shared_ptr<FrameBuffer> shadowFramebuffer;
		bool castsShadow;
		LightType type;
		Vec3 color;
		//Vec3 diffuseColor;
		//Vec3 specularColor;
		//std::uint64_t lightID;
	};
}
