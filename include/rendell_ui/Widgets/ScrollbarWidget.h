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
		void onMouseDown(glm::dvec2 cursorPosition) override;
		void onDragged(glm::dvec2 startPoint, glm::dvec2 endPoint) override;

		float getProgress() const;
		IScrollableWidgetWeakPtr getScrollable() const;

		void updateProgress();
		void setScrollable(IScrollableWidgetWeakPtr scrollable);
		void setProgress(float value);

	private:
		// Widget
		void onSelfWeakPtrChanged() override;

		void processMouseInput(glm::dvec2 cursorPosition);
		void updateHandle();
		float calculateHandleHeight();

		RectangleWidgetSharedPtr _backgroundRectangleWidget;
		RectangleWidgetSharedPtr _handleRectangleWidget;

		float _progress{ 0.0f };
		bool _isHidden{ false };
		glm::dvec2 _lastDragLength{ 0.0f, 0.0f };
		IScrollableWidgetWeakPtr _scrollable{};
	};

	RENDELL_UI_DECLARE_WIDGET(ScrollbarWidget)
}