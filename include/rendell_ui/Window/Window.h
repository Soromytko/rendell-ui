#pragma once
#include <glm/glm.hpp>
#include "window_input.h"
#include "WindowEventHandler.h"

struct GLFWwindow;

namespace rendell_ui
{
	class Window
	{
	public:
		Window(int width, int height, const char* title);
		virtual ~Window();

		bool isOpen();
		void processEvents();
		static bool isInitialized();
		static int getWindowCount();

		void setEventHandler(WindowEventHandlerSharedPtr eventHandler);

		void waitEvent() const;
		void* getNativeWindowHandle() const;
		void* getX11Display() const;
		glm::ivec2 getSize() const;
		glm::dvec2 getCursorPosition() const;
		WindowEventHandlerSharedPtr getEventHandler() const;

	private:
		bool init();
		void setupWindowCallbacks();

	protected:
		GLFWwindow* _glfwWindow;
		WindowEventHandlerSharedPtr _eventHandler{ nullptr };
		static bool _glfwInitialized;
		static int _windowCount;

	};

	RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(Window)
}