#pragma once

namespace Nova {
		
	/**
	* Interface class for Window
	* this class would manage windowing functions for diferent OSes
	* right now it uses just glfw
	*/
	class IWindow
	{
	public:
		IWindow() {};		
		virtual ~IWindow() {};
		//virtual void windowLoop() = 0;		
		virtual int getWidth() = 0;
		virtual int getHeight() = 0;
		virtual bool isClosing() = 0;
		virtual const std::string& getTitle() = 0;
		virtual void setTitle(const std::string &title) = 0;
		virtual void swapFrameBuffers() = 0;
		virtual void processEvents() = 0;
		virtual void minimize() = 0;
		virtual void restore() = 0;
		virtual void show() = 0;
		virtual void showCursor() = 0;
		virtual void hideCursor() = 0;
		virtual bool isCursorShown() = 0;
	};	
}
