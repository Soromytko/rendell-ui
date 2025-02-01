#pragma once
#include <memory>
#include "input.h"

namespace rendell_ui
{
	class WindowEventHandler
	{
	public:
		WindowEventHandler() = default;
		virtual ~WindowEventHandler() = default;

		virtual void onRefreshed(int width, int height) {};
		virtual void onResized(int width, int height) {};
		virtual void onKeyInputted(int key, InputAction active) {};
		virtual void onCharInputted(unsigned char character) {};
	};

	typedef std::shared_ptr<WindowEventHandler> WindowEventHandlerSharedPtr;

}