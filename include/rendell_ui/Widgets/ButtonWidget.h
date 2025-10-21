#pragma once
#include "../Signal.h"
#include "RectangleWidget.h"
#include "Widget.h"

namespace rendell_ui {
class ButtonWidget final : public Widget {
public:
    ButtonWidget();
    ~ButtonWidget() = default;

    void draw() override;

    Signal<void> clicked;

private:
    void onSelfWeakPtrChanged() override;
    void onMouseEntered() override;
    void onMouseExited() override;
    void onMouseClick() override;
    void onCaptured(glm::dvec2 cursorPosition) override;
    void onFreed(glm::dvec2 cursorPosition) override;

    void onColorChanged() override;

    bool _isHovered{false};
    bool _isCaptured{false};

    RectangleWidgetSharedPtr _rectangleWidget;
};

RENDELL_UI_DECLARE_WIDGET(ButtonWidget)
} // namespace rendell_ui
