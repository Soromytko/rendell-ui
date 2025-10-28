#pragma once
#include <rendell_ui/Signal.h>
#include <rendell_ui/Widgets/RectangleWidget.h>
#include <rendell_ui/Widgets/TextWidget.h>
#include <rendell_ui/Widgets/Widget.h>
#include <rendell_ui/defines.h>

namespace rendell_ui {
class DockHeaderWidget final : public RectangleWidget {
public:
    DockHeaderWidget();
    ~DockHeaderWidget() = default;

    // Widget
    void draw() override;
    void onMouseDown(glm::dvec2 cursorPosition) override;
    void onFreed(glm::dvec2 cursorPosition) override;
    void onDragged(glm::dvec2 startPoint, glm::dvec2 endPoint) override;

    void setHeader(const std::string &header);

    Action headerDragged;

private:
    // Widget
    void onSelfWeakPtrChanged() override;

    TextWidgetSharedPtr _textWidget;
};
RENDELL_UI_DECLARE_WIDGET(DockHeaderWidget)

} // namespace rendell_ui