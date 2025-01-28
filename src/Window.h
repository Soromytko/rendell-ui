#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "input.h"
#include "Viewport.h"

namespace rendell_ui
{
	class Window
	{
	public:
		Window(int width, int height, const char* title);
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
		virtual void onKeyInputted(int key, InputAction active) {};

	private:
		bool init();
		void setupWindowCallbacks();

	protected:
		GLFWwindow* _glfwWindow;
		static bool _glfwInitialized;
		static int _windowCount;

	};
}