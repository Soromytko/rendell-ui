#include <rendell_ui/Docking/Widgets/DockRootWidget.h>

namespace rendell_ui {

DockRootWidget::DockRootWidget()
    : Widget() {
    _name = "DockRootWidget";

    _titleBarWidget = createUnregisteredDockTitleBarWidget();
    _titleBarWidget->setName("TitleBarWidget");
    _titleBarWidget->setAnchor(Anchor::topStretch);
    _titleBarWidget->setSize({0.0f, 30.0f});
    _titleBarWidget->setColor({0.18f, 0.18f, 0.18f, 1.0f});

    _workSpaceWidget = createUnregisteredRectangleWidget();
    _workSpaceWidget->setName("WorkSpaceWidget");
    _workSpaceWidget->setAnchor(Anchor::centerStretch);
    _workSpaceWidget->setMargins(Margins::makeTop(_titleBarWidget->getSize().y));
}

void DockRootWidget::draw() {
    _workSpaceWidget->draw();
    _titleBarWidget->draw();
}

DockHeaderWidgetSharedPtr DockRootWidget::getHeaderWidget() const {
    return _titleBarWidget->getHeaderWidget();
}

DockTitleBarWidgetSharedPtr DockRootWidget::getTitleBarWidget() const {
    return _titleBarWidget;
}

WidgetSharedPtr DockRootWidget::getWorkSpaceWidget() const {
    return _workSpaceWidget;
}

void DockRootWidget::onSelfWeakPtrChanged() {
    _workSpaceWidget->setParent(_selfWeakPtr);
    _titleBarWidget->setParent(_selfWeakPtr);
}

} // namespace rendell_ui