#pragma once
#include "Widget.h"
#include "RectangleWidget.h"
#include "IScrollableWidget.h"
#include <rendell_ui/defines.h>

namespace rendell_ui
{
	class ScrollbarWidget : public Widget
	{
	public:
		ScrollbarWidget();
		~ScrollbarWidget() = default;

		//Widget
		void draw() const override;
	private:
		void onCaptured(glm::dvec2 cursorPosition) override;
		void onDragged(glm::dvec2 startPoint, glm::dvec2 endPoint) override;
		void onMouseScrolled(glm::dvec2 scroll) override;

	public:
		double getProgress() const;
		IScrollableWidgetWeakPtr getScrollable() const;

		void updateProgress();
		void setScrollable(IScrollableWidgetWeakPtr scrollable);
		void setProgress(double value);

	private:
		// Widget
		void onSelfWeakPtrChanged() override;

		void startDragging();
		void processDragging(glm::dvec2 offset);
		void updateHandle();
		float calculateHandleHeight();

		RectangleWidgetSharedPtr _backgroundRectangleWidget;
		RectangleWidgetSharedPtr _handleRectangleWidget;

		double _progress{ 0.0 };
		bool _isHidden{ false };
		glm::dvec2 _lastDragLength{ 0.0, 0.0 };
		glm::dvec2 _startDraggingHandleOffset{};
		IScrollableWidgetWeakPtr _scrollable{};
	};

	RENDELL_UI_DECLARE_WIDGET(ScrollbarWidget)
}