#pragma once
#include <memory>
#include <rendell_ui/Widgets/Widget.h>
#include <rendell_ui/Window/WindowEventHandler.h>
#include <rendell_ui/defines.h>

namespace rendell_ui
{
	class RootWidget final : public Widget
	{
	public:
		RootWidget();
		~RootWidget() = default;

		WindowEventHandler* getWindowEventHandler() const;
		void setWindowEventHandler(WindowEventHandler* windowEventHandler);

	private:

		WindowEventHandler* _windowEventHandler{ nullptr };
	};

	RENDELL_UI_DECLARE_WIDGET(RootWidget)
}