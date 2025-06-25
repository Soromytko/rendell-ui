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
}