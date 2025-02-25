#pragma once
#include <set>
#include <memory>
#include "Window/WindowEventHandler.h"
#include "Viewport.h"
#include "Widgets/Widget.h"

namespace rendell_ui
{
	class Canvas : public WindowEventHandler
	{
	public:
		Canvas(ViewportSharedPtr viewport);
		virtual ~Canvas() = default;

		void addWidget(WidgetSharedPtr widget);
		void removeWidget(WidgetSharedPtr widget);

		const std::set<WidgetSharedPtr>& getWidgets() const;
		ViewportSharedPtr getViewport() const;

		virtual void onRefreshed(int width, int height) override;
		virtual void onKeyInputted(InputKey key, InputAction action, InputModControl modControl) override;
		virtual void onMouseButtonInputted(const MouseInput& mouseInput) override;
		virtual void onCharInputted(unsigned char character) override;

	private:
		void setFocusedWidget(Widget* widget);
		void updateWidgetRecursively();
		Widget* focusWidgetRecursively(Widget* widget, glm::vec2 cursor);

		Widget* _focusedWidget{ nullptr };

		ViewportSharedPtr _viewport;
		std::set<WidgetSharedPtr> _widgets;

	};

	typedef std::shared_ptr<Canvas> CanvasSharedPtr;
}