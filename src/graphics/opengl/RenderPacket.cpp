#include <memory>
#include <vector>
#include "RenderPacket.h"
#include "graphics/opengl/Material.h"
#include "graphics/opengl/Mesh.h"
#include "graphics/opengl/GraphicsSystem.h"
#include "graphics/opengl/GPUProgramParameter.h"
#include "ECS/Entity.h"
#include "PreInit.h"
#ifdef NOVA_OPENGL
	#include "graphics/opengl/RendererInit.h"
#endif

namespace Nova {	
	void RenderPacket::draw() const
	{ 
		glDrawElements(GL_TRIANGLES, mMesh->getNumIndices(), GL_UNSIGNED_INT, 0);
	};
	//std::uint64_t RenderPacket::mIDCounter = 0;
	RenderPacket::RenderPacket(const std::shared_ptr<Mesh> & mesh, const std::shared_ptr<Material> & material,
		const Transform * transform)		
		//:mMesh(std::move(mesh)), mMaterial(std::move(material)), mTransform(transform)
		:mMesh(mesh), mMaterial(material), mTransform(transform)
	{
		//mMesh = std::move(mesh); mMaterial = std::move(material); mTransform = std::move(transform);
		//temporary, probably need a custom memory allocator		
		GLuint id = mMaterial->getGPUProgram()->getProgramID();
		auto& gs = GraphicsSystem::getInstance();
		auto& lightsList = GraphicsSystem::getInstance().getLights();
		
		//addParameter(new GPUProgramParameterVec3(glGetUniformLocation(id, "cameraPos"), transform->forward.getDataPtr()));
		addParameter(new GPUProgramParameterMat4(glGetUniformLocation(id, "model"), transform->finalTransform.getDataPtr()));
		addParameter(new GPUProgramParameterMat4(glGetUniformLocation(id, "normalMat"), transform->normalMatrix.getDataPtr()));
		addParameter(new GPUProgramParameterMat4(glGetUniformLocation(id, "view"), gs.getCurrentCamera().viewMatrix->getDataPtr()));
		addParameter(new GPUProgramParameterMat4(glGetUniformLocation(id, "proj"), gs.getCurrentCamera().projectionMatrix->getDataPtr()));

		std::size_t i = 0;
		//for (int i = 0; i < MAX_LIGHTS && i < lightsList.size(); i++)
		for(auto &light : lightsList)
		{
			if (i >= MAX_LIGHTS) break;
				//const int a = it.getTypeCode();
				addParameter(new GPUProgramParameterVec3(
					glGetUniformLocation(id, ("light[" + std::to_string(i) + "].position").c_str()),
					light.getPosition()->getDataPtr()));
				addParameter(new GPUProgramParameterVec3(
					glGetUniformLocation(id, ("light[" + std::to_string(i) + "].color").c_str()),
					light.getColor()->getDataPtr()));
				addParameter(new GPUProgramParameterInt(
					glGetUniformLocation(id, ("light[" + std::to_string(i) + "].type").c_str()),
					light.getTypeCode()));
				/*addParameter(new GPUProgramParameterVec3(
					glGetUniformLocation(id, ("light[" + std::to_string(i) + "].diffuse").c_str()),
					light.getDiffuseColor()->getDataPtr()));
				addParameter(new GPUProgramParameterVec3(
					glGetUniformLocation(id, ("light[" + std::to_string(i) + "].specular").c_str()),
					light.getSpecularColor()->getDataPtr()));
				addParameter(new GPUProgramParameterFloat(
					glGetUniformLocation(id, ("light[" + std::to_string(i) + "].constant").c_str()),
					light.mConstant));
				addParameter(new GPUProgramParameterFloat(
					glGetUniformLocation(id, ("light[" + std::to_string(i) + "].linear").c_str()),
					light.mLinear));
				addParameter(new GPUProgramParameterFloat(
					glGetUniformLocation(id, ("light[" + std::to_string(i) + "].quadratic").c_str()),
					light.mQuadratic));*/
			i++;
		}
		/*addParameter(new GPUProgramParameterVec3(glGetUniformLocation(id, "materialAmbient"), mMaterial->getAmbientColor().getDataPtr()));
		addParameter(new GPUProgramParameterVec3(glGetUniformLocation(id, "materialDiffuse"), mMaterial->getDiffuseColor().getDataPtr()));
		addParameter(new GPUProgramParameterVec4(glGetUniformLocation(id, "materialSpecular"), mMaterial->getSpecularColor().getDataPtr()));*/
	}

	RenderPacket::RenderPacket(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material)
		:mMesh(std::move(mesh)), mMaterial(std::move(material))
		//:RenderPacket()
	{
		auto& gs = GraphicsSystem::getInstance();
		GLuint id = mMaterial->getGPUProgram()->getProgramID();

		addParameter(new GPUProgramParameterMat4(glGetUniformLocation(id, "view"), gs.getCurrentCamera().viewMatrix->getDataPtr()));
		addParameter(new GPUProgramParameterMat4(glGetUniformLocation(id, "proj"), gs.getCurrentCamera().projectionMatrix->getDataPtr()));
		std::size_t i = 0;
		auto& lightsList = GraphicsSystem::getInstance().getLights();
		for (auto &light : lightsList)
		{
			if (i >= MAX_LIGHTS) break;
			
			addParameter(new GPUProgramParameterVec3(
				glGetUniformLocation(id, ("light[" + std::to_string(i) + "].position").c_str()),
				light.getPosition()->getDataPtr()));
			addParameter(new GPUProgramParameterVec3(
				glGetUniformLocation(id, ("light[" + std::to_string(i) + "].color").c_str()),
				light.getColor()->getDataPtr()));
			addParameter(new GPUProgramParameterInt(
				glGetUniformLocation(id, ("light[" + std::to_string(i) + "].type").c_str()),
				light.getTypeCode()));
			/*addParameter(new GPUProgramParameterVec3(
				glGetUniformLocation(id, ("light[" + std::to_string(i) + "].diffuse").c_str()),
				light.getDiffuseColor()->getDataPtr()));
			addParameter(new GPUProgramParameterVec3(
				glGetUniformLocation(id, ("light[" + std::to_string(i) + "].specular").c_str()),
				light.getSpecularColor()->getDataPtr()));
			addParameter(new GPUProgramParameterFloat(
				glGetUniformLocation(id, ("light[" + std::to_string(i) + "].constant").c_str()),
				light.mConstant));
			addParameter(new GPUProgramParameterFloat(
				glGetUniformLocation(id, ("light[" + std::to_string(i) + "].linear").c_str()),
				light.mLinear));
			addParameter(new GPUProgramParameterFloat(
				glGetUniformLocation(id, ("light[" + std::to_string(i) + "].quadratic").c_str()),
				light.mQuadratic));*/
			i++;
		}
	}

	/*todo: this function should be replaced by the ECSystem*/
	void RenderPacket::updateAllUniforms() const
	{
		for (IGPUProgramParameter* parameter : mParameters)
		{
			parameter->update();
		}
	}
		
	RenderPacket::~RenderPacket()
	{
		for (IGPUProgramParameter* parameter : mParameters)
		{
			delete parameter;
		}
	}

	RenderPacket::RenderPacket(RenderPacket && other)
		:mMesh(std::move(other.mMesh)), mMaterial(std::move(other.mMaterial)), mTransform(other.mTransform),
		mParameters(std::move(other.mParameters)) /*mID(other.mID),*/ 
	{
		other.mParameters = std::vector<IGPUProgramParameter*>{};
	}

	RenderPacket & RenderPacket::operator=(RenderPacket && other)
	{
		//return std::move(RenderPacket(std::move(otherGP.mMesh), std::move(otherGP.mMaterial), std::move(otherGP.mTransform)));
		//return std::move(returnValue);
		if (&other != this) {
			mMesh = std::move(other.mMesh);
			mMaterial = std::move(other.mMaterial);
			mParameters = std::move(other.mParameters);
			//otherGP.mParameters = std::vector<IGPUProgramParameter*>{};
			//mID = other.mID;
			mTransform = std::move(other.mTransform);
		}
		return *this;
	}

	void RenderPacket::bind() const
	{
		mMesh->bind();
		mMaterial->bind();
	}
	void RenderPacket::unBind() const
	{
		mMesh->unBind();
		mMaterial->unBind();
	}
}