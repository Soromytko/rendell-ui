#pragma once
#include <memory>
#include "window_input.h"
#include "../defines.h"

namespace rendell_ui
{
	class WindowEventHandler
	{
	public:
		WindowEventHandler() = default;
		virtual ~WindowEventHandler() = default;

		virtual void onRefreshed(int width, int height) {};
		virtual void onResized(int width, int height) {};
		virtual void onKeyInputted(const KeyboardInput& keyboardInput) {};
		virtual void onMouseButtonInputted(const MouseInput& mouseInput) {};
		virtual void onMouseMoved(double x, double y) {};
		virtual void onMouseScrolled(double x, double y) {}
		virtual void onCharInputted(unsigned char character) {};
	};

	DECLARE_SHARED_PTR_FACTORY(WindowEventHandler)

}