#pragma once

namespace Nova {

#if GRAPHICS_API == OPENGL	
	static const GLuint POS_ATTRIB_LOC = 0;
	//static const GLuint NOVA_INDEX_ATTRIBUTE = 1;
	//static const GLuint NOVA_COLOR_ATTRIB = 1;
	static const GLuint TEX_ATTRIB_LOC = 1;
	static const GLuint NORM_ATTRIB_LOC = 2;
	
	GLint getMaxColorAttachments()
	{
		GLint maxColors;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColors);
		return maxColors;
	}
	GLint getMRTMaxBuffers()
	{
		GLint mrtMax;		
		glGetIntegerv(GL_MAX_DRAW_BUFFERS, &mrtMax);
		return mrtMax;
	}
	static const GLuint MAX_COLOR_ATTACHMENTS = getMaxColorAttachments();
	static const GLuint MAX_MRT_BUFFERS = getMRTMaxBuffers();

	static void openglErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam)
	{
		if (type == GL_DEBUG_TYPE_ERROR)
		{
			LOG_ERROR(message)
		}
		else
		{
			//LOG_DEBUG(message);
		}
		/*fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);*/
	}

	static GLenum glCheckError_(const char *file, int line)
	{
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:						error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:						error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:					error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:						error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:					error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:						error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:		error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: error = "FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
			}
			LOG_WARNING(error << " | " << file << " (" << line << ")\n");
		}
		return errorCode;
	}
#define CHECK_OPEGL_ERROR() glCheckError_(__FILE__, __LINE__) 

#endif
	enum LightType{ DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT };
	static const size_t MAX_LIGHTS = 100;
	static const int DEFAULT_SHADOW_MAP_WIDTH = 1024;
	static const int DEFAULT_SHADOW_MAP_HEIGHT = 1024;


}