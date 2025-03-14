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

		void addWidget(const WidgetSharedPtr& widget);
		void removeWidget(const WidgetSharedPtr& widget);
		void focusWidget(const WidgetSharedPtr& widget);

		const std::set<WidgetSharedPtr>& getWidgets() const;
		ViewportSharedPtr getViewport() const;

		virtual void onRefreshed(int width, int height) override;
		virtual void onKeyInputted(const KeyboardInput& keyboardInput) override;
		virtual void onMouseButtonInputted(const MouseInput& mouseInput) override;
		virtual void onCharInputted(unsigned char character) override;

	private:
		void setFocusedWidget(const WidgetSharedPtr& widget);
		void updateWidgetRecursively();
		WidgetSharedPtr focusWidgetRecursively(const WidgetSharedPtr& widget, glm::vec2 cursor);

		WidgetSharedPtr _focusedWidget{ nullptr };

		ViewportSharedPtr _viewport;
		std::set<WidgetSharedPtr> _widgets;

	};

	DECLARE_SHARED_PTR_FACTORY(Canvas)
}