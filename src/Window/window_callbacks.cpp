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
		InputModControl modControl{ mods };
		KeyboardInput keyboardInput
		{
			convertGlfwKeyToInputKey(key),
			convertGlfwActionToInputAction(action),
			modControl,

		};
		evnetHandler->onKeyInputted(keyboardInput);
	}

	void inputMouseBottonCallback(GLFWwindow* glfwWindow, int button, int action, int mods)
	{
		Window* window = GET_WINDOW(glfwWindow);
		const glm::vec2 windowSize = window->getSize();
		const glm::dvec2 cursorPosition = window->getCursorPosition();
		WindowEventHandlerSharedPtr eventHandler = window->getEventHandler();
		MouseInput mouseInput
		{
			convertGlfwMouseButtonToInputMouseButton(button),
			convertGlfwActionToInputAction(action),
			InputModControl{mods},
			cursorPosition.x, cursorPosition.y,
		};
		eventHandler->onMouseButtonInputted(mouseInput);
	}

	void moveMouseCallback(GLFWwindow* glfwWindow, double x, double y)
	{
		const Window* window = GET_WINDOW(glfwWindow);
		const WindowEventHandlerSharedPtr& eventHandler = window->getEventHandler();
		const glm::dvec2 cursor = window->getCursorPosition();
		eventHandler->onMouseMoved(cursor.x, cursor.y);
	}

	void inputCharCallback(GLFWwindow* glfwWindow, unsigned int codepoint)
	{
		WindowEventHandlerSharedPtr evnetHandler = GET_WINDOW(glfwWindow)->getEventHandler();
		evnetHandler->onCharInputted(codepoint);
	}

}
