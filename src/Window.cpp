#include "Window.h"

#define GET_WINDOW(glfwWindow) static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow))

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

	void refreshWindowCallback(GLFWwindow* glfwWindow)
	{
		Window* window = GET_WINDOW(glfwWindow);
		WindowEventHandlerSharedPtr evnetHandler = window->getEventHandler();
		glm::ivec2 windowSize = window->getSize();
		evnetHandler->onRefreshed(windowSize.x, windowSize.y);
	}

	void resizeWindowCallback(GLFWwindow* glfwWindow, int width, int height)
	{
		WindowEventHandlerSharedPtr evnetHandler = GET_WINDOW(glfwWindow)->getEventHandler();
		evnetHandler->onResized(width, height);
	}

	void inputKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
	{
		WindowEventHandlerSharedPtr evnetHandler = GET_WINDOW(glfwWindow)->getEventHandler();
		InputModControl modControl{ mods };
		evnetHandler->onKeyInputted(key, convertGlfwActionToInputAction(action), modControl);
	}

	void inputCharCallback(GLFWwindow* glfwWindow, unsigned int codepoint)
	{
		WindowEventHandlerSharedPtr evnetHandler = GET_WINDOW(glfwWindow)->getEventHandler();
		evnetHandler->onCharInputted(codepoint);
	}

	void Window::setupWindowCallbacks()
	{
		if (!_eventHandler)
		{
			if (!s_eventHandlerStub)
			{
				s_eventHandlerStub = std::make_shared<WindowEventHandler>();
			}
			_eventHandler = s_eventHandlerStub;
		}
		glfwSetWindowUserPointer(_glfwWindow, static_cast<void*>(this));

		glfwSetFramebufferSizeCallback(_glfwWindow, resizeWindowCallback);
		glfwSetWindowRefreshCallback(_glfwWindow, refreshWindowCallback);
		glfwSetKeyCallback(_glfwWindow, inputKeyCallback);
		glfwSetCharCallback(_glfwWindow, inputCharCallback);
	}

	bool Window::_glfwInitialized = false;
	int Window::_windowCount = 0;
}
