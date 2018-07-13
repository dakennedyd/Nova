#pragma once
#include <glad/glad.h>
#include "PreInit.h"
#include "Init.h"

namespace Nova {

#ifdef NOVA_OPENGL
	static const GLuint POS_ATTRIB_LOC = 0;
	//static const GLuint NOVA_INDEX_ATTRIBUTE = 1;
	//static const GLuint NOVA_COLOR_ATTRIB = 1;
	static const GLuint TEX_ATTRIB_LOC = 1;
	static const GLuint NORM_ATTRIB_LOC = 2;
	
	GLint getMaxColorAttachments();
	GLint getMRTMaxBuffers();
	//static const GLuint MAX_COLOR_ATTACHMENTS = getMaxColorAttachments();
	//static const GLuint MAX_MRT_BUFFERS = getMRTMaxBuffers();
	static const std::string RENDERER{"opengl"};
	static const std::string SHADERS_PATH(RESOURCES_PATH + "shaders/glsl/");

	void openglErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam);

	static GLenum glCheckError_(const char *file, int line);
	#define CHECK_OPEGL_ERROR() glCheckError_(__FILE__, __LINE__) 

#endif
	enum LightType{ DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT };
	static const size_t MAX_LIGHTS = 100;
	static const int DEFAULT_SHADOW_MAP_WIDTH = 1024;
	static const int DEFAULT_SHADOW_MAP_HEIGHT = 1024;


}