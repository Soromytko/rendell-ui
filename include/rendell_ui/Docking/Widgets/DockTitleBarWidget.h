#pragma once
#include "DockHeaderWidget.h"
#include <rendell_ui/Widgets/RectangleWidget.h>
#include <rendell_ui/defines.h>

namespace rendell_ui {
class DockTitleBarWidget final : public RectangleWidget {
public:
    DockTitleBarWidget();
    ~DockTitleBarWidget() = default;

    // Widget
    void draw() override;

    DockHeaderWidgetSharedPtr getHeaderWidget() const;

private:
    // Widget
    void onSelfWeakPtrChanged() override;

    DockHeaderWidgetSharedPtr _headerWidget;
};

RENDELL_UI_DECLARE_WIDGET(DockTitleBarWidget)

} // namespace rendell_ui