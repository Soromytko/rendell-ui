#include <rendell_ui/Window/Window.h>
#include <GLFW/glfw3.h>
#include "window_callbacks.h"

#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#endif

namespace rendell_ui
{
	static WindowEventHandlerSharedPtr s_eventHandlerStub;

	Window::Window(int width, int height, const char* title)
	{
		if (!init())
		{
			return;
		}
		_glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!_glfwWindow)
		{
			return;
		}
		_windowCount++;

		setupWindowCallbacks();
	}

	static void release_glfw_cursor(GLFWcursor* glfwCursor)
	{
		if (glfwCursor)
		{
			glfwDestroyCursor(glfwCursor);
		}
	}

	Window::~Window()
	{
		if (_glfwWindow)
		{
			_windowCount--;
			if (_windowCount <= 0)
			{
				glfwTerminate();
				_glfwInitialized = false;
			}
		}

		release_glfw_cursor(_glfwCursor);
	}

	WindowCursorType Window::getCursorType() const
	{
		return  _cursorType;
	}

	static int get_glfw_cursor_type(WindowCursorType type)
	{
		switch (type)
		{
		case WindowCursorType::arrow: return GLFW_ARROW_CURSOR;
		case WindowCursorType::hand: return GLFW_HAND_CURSOR;
		case WindowCursorType::cross: return GLFW_CROSSHAIR_CURSOR;
		case WindowCursorType::ibeam: return GLFW_IBEAM_CURSOR;
		}

		return GLFW_ARROW_CURSOR;
	}

	void Window::setCursorType(WindowCursorType type)
	{
		if (_cursorType != type)
		{
			_cursorType = type;

			release_glfw_cursor(_glfwCursor);
			_glfwCursor = glfwCreateStandardCursor(get_glfw_cursor_type(type));
			glfwSetCursor(_glfwWindow, _glfwCursor);
		}
	}

	bool Window::isOpen()
	{
		return !glfwWindowShouldClose(_glfwWindow);
	}

	void Window::processEvents()
	{
		glfwPollEvents();
	}

	bool Window::isInitialized()
	{
		return _glfwInitialized;
	}

	int Window::getWindowCount()
	{
		return _windowCount;
	}

	void Window::setEventHandler(WindowEventHandlerSharedPtr eventHandler)
	{
		_eventHandler = eventHandler;
		_eventHandler->setWindow(this);
	}

	static void* get_native_window_handle(GLFWwindow* window)
	{
#if defined(_WIN32)
		return (void*)glfwGetWin32Window(window);
#elif defined(__linux__)
		return (void*)(uintptr_t)glfwGetX11Window(window);
#endif
		return nullptr;
	}

	void Window::waitEvent() const
	{
		glfwWaitEvents();
	}

	void* Window::getNativeWindowHandle() const
	{
		return get_native_window_handle(_glfwWindow);
	}

	void* Window::getX11Display() const
	{
#if defined(__linux__)
		return (void*)glfwGetX11Display();
#endif
		return nullptr;
	}

	glm::ivec2 Window::getSize() const
	{
		int width, height;
		glfwGetFramebufferSize(_glfwWindow, &width, &height);
		return glm::ivec2(width, height);
	}

	glm::dvec2 Window::getCursorPosition() const
	{
		double x, y;
		glfwGetCursorPos(_glfwWindow, &x, &y);
		return (static_cast<glm::dvec2>(getSize()) * 0.5 - glm::dvec2(x, y)) * glm::dvec2(-1.0, 1.0);
	}

	WindowEventHandlerSharedPtr Window::getEventHandler() const
	{
		return _eventHandler;
	}

	bool Window::init()
	{
		if (!_glfwInitialized)
		{
#if defined(__linux__)
			glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			_glfwInitialized = glfwInit();
		}
		return _glfwInitialized;
	}

	void Window::setupWindowCallbacks()
	{
		if (!_eventHandler)
		{
			if (!s_eventHandlerStub)
			{
				s_eventHandlerStub = makeWindowEventHandler();
			}
			_eventHandler = s_eventHandlerStub;
		}
		glfwSetWindowUserPointer(_glfwWindow, static_cast<void*>(this));

		glfwSetFramebufferSizeCallback(_glfwWindow, resizeWindowCallback);
		glfwSetWindowRefreshCallback(_glfwWindow, refreshWindowCallback);
		glfwSetKeyCallback(_glfwWindow, inputKeyCallback);
		glfwSetMouseButtonCallback(_glfwWindow, inputMouseBottonCallback);
		glfwSetCursorPosCallback(_glfwWindow, moveMouseCallback);
		glfwSetCursorEnterCallback(_glfwWindow, enterCursorCallback);
		glfwSetScrollCallback(_glfwWindow, scrollMouseCallback);
		glfwSetCharCallback(_glfwWindow, inputCharCallback);
	}

	bool Window::_glfwInitialized = false;
	int Window::_windowCount = 0;
}
