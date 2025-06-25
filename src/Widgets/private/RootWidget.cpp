#include <Widgets/RootWidget.h>

namespace rendell_ui
{
	RootWidget::RootWidget() : Widget()
	{
		_anchor = Anchor::centerStretch;
	}

	WindowEventHandler* RootWidget::getWindowEventHandler() const
	{
		return _windowEventHandler;
	}

	void RootWidget::setWindowEventHandler(WindowEventHandler* windowEventHandler)
	{
		_windowEventHandler = windowEventHandler;
	}

	WindowCursorType RootWidget::getWindowCursorType() const
	{
		return _windowEventHandler->getWindowCursorType();
	}

	void RootWidget::setWindowCursorType(WindowCursorType type)
	{
		_windowEventHandler->setWindowCursor(type);
	}

}