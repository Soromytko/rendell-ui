#pragma once

struct GLFWwindow;

namespace rendell_ui {
void refreshWindowCallback(GLFWwindow *glfwWindow);

void resizeWindowCallback(GLFWwindow *glfwWindow, int width, int height);

void inputKeyCallback(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods);

void inputCharCallback(GLFWwindow *glfwWindow, unsigned int codepoint);

void inputMouseBottonCallback(GLFWwindow *glfwWindow, int button, int action, int mods);

void moveMouseCallback(GLFWwindow *glfwWindow, double x, double y);

void enterCursorCallback(GLFWwindow *glfwWindow, int entered);

void scrollMouseCallback(GLFWwindow *glfwWindow, double x, double y);

} // namespace rendell_ui
