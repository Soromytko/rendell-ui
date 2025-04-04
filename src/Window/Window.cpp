#include <rendell_ui/Window/Window.h>
#include "window_callbacks.h"

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
	}

	bool Window::isOpen()
	{
		return !glfwWindowShouldClose(_glfwWindow);
	}

	void Window::makeContextCurrent()
	{
		glfwMakeContextCurrent(_glfwWindow);
	}

	void Window::swapBuffers()
	{
		glfwSwapBuffers(_glfwWindow);
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
		glfwSetScrollCallback(_glfwWindow, scrollMouseCallback);
		glfwSetCharCallback(_glfwWindow, inputCharCallback);
	}

	bool Window::_glfwInitialized = false;
	int Window::_windowCount = 0;
}
