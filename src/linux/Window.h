#pragma once
#include "IWindow.h"
//#include <GL/glew.h> // GL extension handler library

namespace Nova {
	class Window final :public IWindow, public ISingleton<Window>, public ISubSystem
	{
	public:
		Window() {};		
		~Window() {};
		void startUp() override;
		void shutDown() override;
		int getWidth() override;
		int getHeight() override;
		bool isClosing() override;
		const std::string& getTitle() override { return mTitle; };
		void setTitle(const std::string &title) override;
		void swapFrameBuffers() override { glfwSwapBuffers(mGLFWindow); };
		void processEvents() override { glfwPollEvents(); };
		void minimize() override { glfwIconifyWindow(mGLFWindow); };
		void restore() override { glfwRestoreWindow(mGLFWindow); };
		void show() override { glfwShowWindow(mGLFWindow); };
		void showCursor() override { glfwSetInputMode(mGLFWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); mCursorEnabled = true; };
		void hideCursor() override { glfwSetInputMode(mGLFWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); mCursorEnabled = false; };
		bool isCursorShown() override { return mCursorEnabled; };
		//void close() { glfwDestroyWindow(mGLFWindow); }
		GLFWwindow* mGLFWindow; //pointer need it by glfw 			
	private:	
		int mWidth;
		int mHeight;
		std::string mTitle;	
		Timer mTimer;
		bool mCursorEnabled;
	};	
}
