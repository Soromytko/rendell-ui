#pragma once
#include <set>
#include <memory>
#include "WindowEventHandler.h"
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

		virtual void onRefreshed(int width, int height);

	private:
		ViewportSharedPtr _viewport;
		std::set<WidgetSharedPtr> _widgets;

	};

	typedef std::shared_ptr<Canvas> CanvasSharedPtr;
}