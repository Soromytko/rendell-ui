#pragma once
#include "Widget.h"
#include "RectangleWidget.h"
#include "../Signal.h"

namespace rendell_ui
{
	class ButtonWidget final : public Widget
	{
	public:
		ButtonWidget();
		~ButtonWidget() = default;

		void draw() const override;

		Signal<void> clicked;

	private:
		void onSelfWeakPtrChanged() override;
		void onMouseEntered() override;
		void onMouseExited() override;
		void onMouseClick() override;
		void onCaptured(glm::dvec2 cursorPosition) override;
		void onFreed(glm::dvec2 cursorPosition) override;

		void onColorChanged() override;

		bool _isHovered{ false };
		bool _isCaptured{ false };

		RectangleWidgetSharedPtr _rectangleWidget;
	};

	RENDELL_UI_DECLARE_WIDGET(ButtonWidget)
}
