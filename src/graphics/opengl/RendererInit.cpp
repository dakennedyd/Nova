#include <glad/glad.h>
#include "RendererInit.h"
#include "logger/Logger.h"

namespace Nova {
#ifdef NOVA_OPENGL
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
    void openglErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
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

	GLenum glCheckError_(const char *file, int line)
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

#endif
}