#pragma once
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
    void draw() const override;

    void setHeader(const std::string &header);

private:
    // Widget
    void onSelfWeakPtrChanged() override;

    TextWidgetSharedPtr _textWidget;
};
RENDELL_UI_DECLARE_WIDGET(DockHeaderWidget)

} // namespace rendell_ui