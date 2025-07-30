#include <rendell_ui/Docking/Widgets/DockHeaderWidget.h>

namespace rendell_ui {
DockHeaderWidget::DockHeaderWidget()
    : RectangleWidget() {
    _textWidget = createUnregisteredTextWidget();
    _textWidget->setAnchor(Anchor::leftStretch);
    _textWidget->setFontSize({20, 20});
}
void DockHeaderWidget::draw() const {
    RectangleWidget::draw();
    _textWidget->draw();
}

void DockHeaderWidget::setHeader(const std::string &header) {
    _textWidget->setText(std::wstring(header.begin(), header.end()));
}

void DockHeaderWidget::onSelfWeakPtrChanged() {
    _textWidget->setParent(_selfWeakPtr);
}
} // namespace rendell_ui