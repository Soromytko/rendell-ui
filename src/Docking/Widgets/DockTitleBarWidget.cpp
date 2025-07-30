#include <rendell_ui/Docking/Widgets/DockTitleBarWidget.h>

namespace rendell_ui {

DockTitleBarWidget::DockTitleBarWidget()
    : RectangleWidget() {

    _headerWidget = createUnregisteredDockHeaderWidget();
    _headerWidget->setAnchor(Anchor::leftStretch);
    _headerWidget->setColor({0.09f, 0.09f, 0.09f, 1.0f});
}

void DockTitleBarWidget::draw() const {
    RectangleWidget::draw();
    _headerWidget->draw();
}

DockHeaderWidgetSharedPtr DockTitleBarWidget::getHeaderWidget() const {
    return _headerWidget;
}

void DockTitleBarWidget::onSelfWeakPtrChanged() {
    _headerWidget->setParent(_selfWeakPtr);
}

} // namespace rendell_ui