#pragma once

namespace rendell_ui
{
	enum class WindowCursorType
	{
		arrow, hand, cross, ibeam,
	};

	class IWindow
	{
	protected:
		IWindow() = default;
	public:
		~IWindow() = default;

		virtual WindowCursorType getCursorType() const = 0;

		virtual void setCursorType(WindowCursorType type) = 0;

	};
}