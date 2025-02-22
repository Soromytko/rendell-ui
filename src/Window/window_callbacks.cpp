#include "window_callbacks.h"
#include "Window.h"

#define GET_WINDOW(glfwWindow) static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow))

namespace rendell_ui
{
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
		InputKey inputKey = convertGlfwKeyToInputKey(key);
		InputModControl modControl{ mods };
		evnetHandler->onKeyInputted(inputKey, convertGlfwActionToInputAction(action), modControl);
	}

	void inputMouseBottonCallback(GLFWwindow* glfwWindow, int button, int action, int mods)
	{
		WindowEventHandlerSharedPtr eventHandler = GET_WINDOW(glfwWindow)->getEventHandler();
		double x, y;
		glfwGetCursorPos(glfwWindow, &x, &y);
		MouseInput mouseInput
		{
			convertGlfwMouseButtonToInputMouseButton(button),
			convertGlfwActionToInputAction(action),
			InputModControl{mods},
			x, y,
		};
		eventHandler->onMouseButtonInputted(mouseInput);
	}

	void inputCharCallback(GLFWwindow* glfwWindow, unsigned int codepoint)
	{
		WindowEventHandlerSharedPtr evnetHandler = GET_WINDOW(glfwWindow)->getEventHandler();
		evnetHandler->onCharInputted(codepoint);
	}

}
