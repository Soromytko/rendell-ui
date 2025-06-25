#pragma once
#include <glm/glm.hpp>
#include "IWindow.h"
#include "window_input.h"
#include "WindowEventHandler.h"

struct GLFWwindow;
struct GLFWcursor;

namespace rendell_ui
{
	class Window : public IWindow
	{
	public:
		Window(int width, int height, const char* title);
		virtual ~Window();

		// IWindow
		WindowCursorType getCursorType() const override;
		void setCursorType(WindowCursorType type) override;

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
		GLFWcursor* _glfwCursor{ nullptr };
		WindowEventHandlerSharedPtr _eventHandler{ nullptr };
		static bool _glfwInitialized;
		static int _windowCount;
		WindowCursorType _cursorType{ WindowCursorType::arrow };

	};

	RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(Window)
}