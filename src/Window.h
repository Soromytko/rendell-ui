#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "IRenderingContext.h"
#include "Viewport.h"

class Window
{
public:
	Window(int width, int height, const char *title);
	virtual ~Window();

	bool isOpen();
	void makeContextCurrent();
	void swapBuffers();
	void processEvents();
	static bool isInitialized();
	static int getWindowCount();

	glm::ivec2 getSize() const;

	virtual void onRefreshed() {};
	virtual void onResized(int width, int height) {};

private:
	bool init();
	void setupWindowCallbacks();

protected:
	GLFWwindow* _glfwWindow;
	static bool _glfwInitialized;
	static int _windowCount;

};