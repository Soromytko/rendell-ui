#pragma once
#include <memory>
#include <rendell_ui/defines.h>
#include "window_input.h"

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
		virtual void onCursorEntered(bool entered) {}
		virtual void onMouseScrolled(double x, double y) {}
		virtual void onCharInputted(unsigned char character) {};
	};

	RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(WindowEventHandler)

}