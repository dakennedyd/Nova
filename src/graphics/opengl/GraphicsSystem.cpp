#include <GLFW/glfw3.h>
#include "GraphicsSystem.h"
#include "logger/Logger.h"

namespace Nova {		
	//RendererBackend GraphicsSystem::mRenderer;
	void GraphicsSystem::startUp()
	{
		LOG_INFO("Initiating graphics subsystem");
		//mCurrentCamera = new FPSCamera();
		//================================================================
		//OPENGL initialization
		//================================================================						
		//glfwSetKeyCallback(mWindow, keyCallback); //put keyboard handlers on keyboard.cpp
		//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		// start GLEW extension handler
		//glewExperimental = GL_TRUE;		
		//glewInit();
		
		// get version info
		const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
		const GLubyte* version = glGetString(GL_VERSION); // version as a string
		const GLubyte* vendor = glGetString(GL_VENDOR);		
		LOG_INFO("RendererBackend:" << renderer);
		LOG_INFO("OpenGL version:" << version << " Vendor:" << vendor);
		checkExtensionsSupport();
		// tell GL to only draw onto a pixel if the shape is closer to the viewer
		glEnable(GL_DEPTH_TEST); // enable depth-testing
		//glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"								
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); // cull back face
		glFrontFace(GL_CW); // GL_CCW for counter clock-wise
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		//glEnable(GL_STENCIL_TEST);
		//glStencilFunc(GL_EQUAL, 1, 0xFF);//discard fragments where stencil is not equal to 1
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //set rasterizer to only draw lines(wireframe)
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
		
		//glViewport(0, 0, Window::getInstance().getWidth(), Window::getInstance().getHeight());	

		//#ifndef NDEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback((GLDEBUGPROC)openglErrorCallback, 0);
		//#else
			//glDisable(GL_DEBUG_OUTPUT);
		//#endif
	}

	void GraphicsSystem::shutDown()
	{
	}

	/*void GraphicsSystem::setSkyBox(RenderPacket * skyBox)
	{
		mCurrentSkyBox = skyBox;
	}*/
	/*Camera * GraphicsSystem::getCurrentCamera()
	{
		return mCurrentCamera;
	}
	void GraphicsSystem::setCurrentCamera(Camera * newCamera)
	{
		mCurrentCamera = newCamera;
	}*/
	void GraphicsSystem::checkExtensionsSupport()
	{
		GLint num = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &num);		
		for (GLint i = 0; i < num; i++)
		{			
			//const char *e = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
			std::string extention(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)));
			//int strSize = 0;
			//while(e[strSize] != NULL)			
				//strSize++;		
			//std::string ext(*e, strSize);
			//ext.erase(std::remove(ext.begin(), ext.end(), ' '), ext.end()); //remove white space
			if(extention == "GL_EXT_texture_filter_anisotropic")
			//if (strcmp(e, "GL_EXT_texture_filter_anisotropic") == 0)
			{				
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &mMaxAnisotropyLevel);			
				//LOG_INFO((const char*)e << " " << mMaxAnisotropyLevel);
			}
		}
		LOG_INFO("Max anisotropy level:" << mMaxAnisotropyLevel);
	}
	GLfloat GraphicsSystem::getMaxAnisotropy()
	{
		return mMaxAnisotropyLevel;
	}
	void GraphicsSystem::setWireframeMode(bool state)
	{
		if (state == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //set rasterizer to only draw lines(wireframe)
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	/*RendererBackend & GraphicsSystem::getRendererBackend()
	{
		return *mRenderer;
	}*/
}