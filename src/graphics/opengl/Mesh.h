#pragma once
#include <vector>
//#include <glad/glad.h>
#include "graphics/IMesh.h"
#include "math/Vector.h"

namespace Nova {
	struct Box {};
	struct Sphere {};
	class Mesh : public IMesh
	{
	public:
		Mesh() = delete;	

		/*template<typename... Params >
		Mesh(const std::string &name, Params&&... parameters)
			:Mesh(init(name, std::forward<Params>(parameters)...)) {}*/

		Mesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices,
			const Vec3 &color = Vec3(1.0f, 1.0f, 1.0f), bool isDoubleSided = true);

		Mesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices, const std::vector<GLfloat> &normals,
			const Vec3 &color = Vec3(1.0f, 1.0f, 1.0f), bool isDoubleSided = true);

		Mesh(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices, const std::vector<GLfloat> &normals,
			const std::vector<GLfloat> &textureCoordinates,
			const Vec3 &color = Vec3(1.0f, 1.0f, 1.0f), bool isDoubleSided = true);

		//rule of five
		~Mesh() { destroyData(); };		
		Mesh(const Mesh &mesh) = delete;					//copy ctor
		Mesh operator =(const Mesh &otherMesh) = delete;	//copy assignment op	
		Mesh(Mesh&& otherMesh);								//move ctor
		Mesh& operator=(Mesh&&) = delete;					//move assignment op

		void bind() const override;
		void unBind() const override;		
		void setColor(const Vec3& color);
		const Vec3 &getColor() const;

		const GLsizei getNumVertices() const;
		const GLsizei getNumIndices() const;
		const GLsizei getMode() const;		
		
		//sets the data for the mesh (needed by the resource manager) (this is ugly)
		/*void setData(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices);
		void setData(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices, const std::vector<GLfloat> &normals);
		void setData(const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices, const std::vector<GLfloat> &normals,
			const std::vector<GLfloat> &textureCoordinates);*/

		static Mesh makeBox(const float length = 1.0f, const float width = 1.0f, const float height = 1.0f,
			const std::vector<GLfloat> &textureCoordinates = std::vector<float>{
			1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f });
		static Mesh makeIcosahedron(const float size = 1.0f);
		static Mesh makeSkyBoxMesh();
		static Mesh makeIcoSphere(const float radius = 1.0f, const int iterations = 1);
		static Mesh makeQuad(const float width = 1.0f, const float height = 1.0f,
			const std::vector<GLfloat> &textureCoordinates = std::vector<float>{
			1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f });

		//static Mesh MeshTest(const int a, const int b) { return makeBox(); };
		//static Mesh MeshTest2(const int a, const int b) { return makeBox(); };		
	private:
		///*template<typename... Args>
		//static const constexpr std::size_t length(Args...) { return sizeof...(Args); }

		//template<typename... Params>
		//static Mesh init(const std::string &name, Params&&... parameters)
		//{
		//	//if constexpr(std::is_same<Tag, Box>::value) return makeBox(std::forward<Params>(parameters)...);
		//	//else if constexpr(std::is_same<Tag, Sphere>::value) return makeSphere(std::forward<Params>(parameters)...);
		//	//std::unordered_map<std::string, Mesh(*) (Params...)> map;
		//	//map.emplace("box", Mesh::makeBox(std::forward<Params>(parameters)...));
		//	//map["box"] = &Mesh::makeBox(std::forward<Params>(parameters)...);
		//	//map["sphere"] = &Mesh::makeIcoSphere(std::forward<Params>(parameters)...);
		//	//map["quad"] = &Mesh::makeQuad(std::forward<Params>(parameters)...);
		//	//const constexpr std::size_t paramsLength = length(std::forward<Params>(parameters)...);
		//	//LOG_DEBUG("n of params:" << paramsLength);
		//	//return (str == "box") ? Mesh::makeBox(std::forward<Params>(parameters)...):(str == "sphere" /*&& paramsLength < 3*/) ? Mesh::makeIcoSphere(std::forward<Params>(parameters)...):Mesh::makeBox(.5f, .5f, .5f);
		//	//return ((str == "box") ? Mesh::makeBox(std::forward<Params>(parameters)...) : Mesh::makeIcoSphere(0.5f, 0));
		//	//std::tuple<Params...> paramTuple;
		//	//int p[] = { 0, ((void)(std::forward<Params>(parameters)), 0) ... };				
		//	//int p[] = { 0, ((void)(std::get<parameters>(paramTuple)), 0)... };			
		//	/*if (str == "box") return Mesh::makeBox(std::forward<Params>(parameters)...);
		//	if (str == "sphere") return Mesh::makeIcoSphere(std::forward<Params>(parameters)...);
		//	if (str == "quad") return Mesh::makeQuad(std::forward<Params>(parameters)...);*/
		//	/*return Mesh::makeBox(.5f, .5f, .5f);
		//	
		//}*/
		struct MeshState
		{
			GLuint verticesVBO = 0;
			GLuint EBO = 0;
			GLuint textureCoordsVBO = 0;
			GLuint normalsVBO = 0;
			GLenum mode = GL_TRIANGLES;
			bool enabledVertexAttributes[16] = {false};
			bool doubleSided = true; //unsused for the moment
		};
		GLuint mVAO;
		GLsizei mSize;								// number of elements in mesh array		
		GLsizei mNumIndices;
		MeshState mState;
		Vec3 mColor;

		void destroyData();	//delete the mesh data from GPU memory
		void enableVertexAttributes();
		void disableVertexAttributes();							
		void constructVertices(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
		void constructNormals(const std::vector<GLfloat>& normals);
		void constructTextureCoordinates(const std::vector<GLfloat>& textureCoordinates);
		void constructVAO();
	};
}
