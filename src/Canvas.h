#pragma once
#include <set>
#include <memory>
#include <unordered_set>
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
		virtual void onMouseMoved(double x, double y) override;
		virtual void onMouseScrolled(double x, double y) override;
		virtual void onCharInputted(unsigned char character) override;

	private:
		void setFocusedWidget(const WidgetSharedPtr& widget);
		bool setCapturedWidget(const WidgetSharedPtr& widget);
		void updateWidgetRecursively();
		WidgetSharedPtr focusWidgetRecursively(const WidgetSharedPtr& widget, glm::vec2 cursor);
		WidgetSharedPtr hoverMouseRecursively(const WidgetSharedPtr& widget, glm::dvec2 cursor);

		WidgetSharedPtr _focusedWidget{ nullptr };
		WidgetSharedPtr _capturedWidget{ nullptr };
		WidgetSharedPtr _hoveredWidget{ nullptr };

		ViewportSharedPtr _viewport;
		std::set<WidgetSharedPtr> _widgets;
		std::unordered_set<WidgetSharedPtr> _mouseHoverWidgets;

	};

	DECLARE_SHARED_PTR_FACTORY(Canvas)
}