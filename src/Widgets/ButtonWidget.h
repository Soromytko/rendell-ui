#pragma once
#include "RectangleWidget.h"

namespace rendell_ui
{
	class ButtonWidget : public RectangleWidget
	{
		FRIEND_WIDGET
	private:
		ButtonWidget() = default;

	public:
		~ButtonWidget() = default;

	private:
		void onFocused() override;
		void onMouseEntered() override;
		void onMouseExited() override;
		void onMouseDown(glm::dvec2 cursorPosition) override;
		void onMouseUp(glm::dvec2 cursorPosition) override;
		void onCaptured() override;
		void onFreed() override;
		void onMouseClick() override;

		bool _isCaptured{};
		bool _isHovered{};
	};

	DECLARE_WIDGET(ButtonWidget)
}