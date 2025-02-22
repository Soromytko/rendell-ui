#pragma once
#include <memory>
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
		virtual void onKeyInputted(InputKey key, InputAction active, InputModControl modControl) {};
		virtual void onMouseButtonInputted(const MouseInput& mouseInput) {};
		virtual void onCharInputted(unsigned char character) {};
	};

	typedef std::shared_ptr<WindowEventHandler> WindowEventHandlerSharedPtr;

}