#pragma once
#include "DockHeaderWidget.h"
#include "DockTitleBarWidget.h"
#include <rendell_ui/Widgets/RectangleWidget.h>
#include <rendell_ui/Widgets/Widget.h>
#include <rendell_ui/defines.h>

namespace rendell_ui {
class DockRootWidget final : public Widget {

public:
    DockRootWidget();
    ~DockRootWidget() = default;

    // Widget
    void draw() const override;

    DockHeaderWidgetSharedPtr getHeaderWidget() const;
    DockTitleBarWidgetSharedPtr getTitleBarWidget() const;
    WidgetSharedPtr getWorkSpaceWidget() const;

private:
    // Widget
    void onSelfWeakPtrChanged() override;

    DockTitleBarWidgetSharedPtr _titleBarWidget;
    RectangleWidgetSharedPtr _workSpaceWidget;
};

RENDELL_UI_DECLARE_WIDGET(DockRootWidget)

} // namespace rendell_ui