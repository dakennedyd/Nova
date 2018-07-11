#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Init.h"
#include "Settings.h"
#include "Error.h"

namespace Nova {		
	void window_close_callback(GLFWwindow* window)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);		
	}

	void Window::startUp()
	{		
		EngineSettings & es = EngineSettings::getInstance();
		//GLFW requires this to be set before window creation		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		if (!glfwInit()) {
			error("could not start GLFW3");
		}
		//this hints have to be set after glfwInit for some reason
		//glfwWindowHint(GLFW_VISIBLE, GL_FALSE);		
		glfwWindowHint(GLFW_SAMPLES, 0);
		// if(!gladLoadGL()) {
		// 	error("could not start GLAD");
    	// }
		
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		GLFWmonitor* mon = glfwGetPrimaryMonitor();
		const GLFWvidmode* vmode = glfwGetVideoMode(mon);
		
		//glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
		if (es.getBoolean("Video", "fullscreen"))
		{
			mWidth = vmode->width;
			mHeight = vmode->height;
			mGLFWindow = glfwCreateWindow(mWidth, mHeight, "Nova", mon, nullptr);
		}
		else {			
			mWidth = es.getInteger("Video", "width");
			mHeight = es.getInteger("Video", "height");
			mGLFWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
		}
		if (!mGLFWindow) {
			glfwTerminate();
			error("could not open window with GLFW3");
		}
		
		setTitle(NOVA_DESCRIPTION_STRING);
		//hideCursor();
		glfwSetWindowCloseCallback(mGLFWindow, window_close_callback);		
		glfwSetWindowPos(mGLFWindow, 50, 50);
		//glfwSetCursorPosCallback(mGLFWindow, handleMouse);	
		glfwSwapInterval(0);
		glfwMakeContextCurrent(mGLFWindow);		
	}

	void Window::shutDown()
	{
		glfwDestroyWindow(mGLFWindow);  //not necesarily needed, glfwTerminate does the same
										//but good to have in case i implement multiple windows
		glfwTerminate();
	}

	int Window::getWidth()
	{
		return mWidth;
	}

	int Window::getHeight()
	{
		return mHeight;
	}

	bool Window::isClosing()
	{
		if (glfwWindowShouldClose(mGLFWindow) == 1) {
			return true;
		}
		else {
			return false;
		}
	}
	
	void Window::setTitle(const std::string &title)
	{		
		//glfwSetWindowTitle(mGLFWindow, std::string("NOVA | FPS:" +
			//std::to_string(static_cast<int>(1.0f / (mTimer.getMillis() / 1000.0f)))).c_str());
		glfwSetWindowTitle(mGLFWindow, title.c_str());
	}	
}
