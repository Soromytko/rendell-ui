#pragma once
#include <GLFW/glfw3.h>

namespace rendell_ui
{
	void refreshWindowCallback(GLFWwindow* glfwWindow);

	void resizeWindowCallback(GLFWwindow* glfwWindow, int width, int height);

	void inputKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);

	void inputCharCallback(GLFWwindow* glfwWindow, unsigned int codepoint);
}
