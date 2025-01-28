#include "Window.h"

#define GET_WINDOW(glfwWindow) static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow))

namespace rendell_ui
{
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

	glm::ivec2 Window::getSize() const
	{
		int width, height;
		glfwGetFramebufferSize(_glfwWindow, &width, &height);
		return glm::ivec2(width, height);
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
		window->onRefreshed();
	}

	void resizeWindowCallback(GLFWwindow* glfwWindow, int width, int height)
	{
		Window* window = GET_WINDOW(glfwWindow);
		window->onResized(width, height);
	}

	void inputKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
	{
		Window* window = GET_WINDOW(glfwWindow);
		window->onKeyInputted(key, convertGlfwActionToInputAction(action));
	}

	void Window::setupWindowCallbacks()
	{
		glfwSetWindowUserPointer(_glfwWindow, static_cast<void*>(this));

		glfwSetFramebufferSizeCallback(_glfwWindow, resizeWindowCallback);
		glfwSetWindowRefreshCallback(_glfwWindow, refreshWindowCallback);
		glfwSetKeyCallback(_glfwWindow, inputKeyCallback);
	}

	bool Window::_glfwInitialized = false;
	int Window::_windowCount = 0;
}
