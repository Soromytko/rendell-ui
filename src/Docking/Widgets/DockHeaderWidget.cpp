#include <rendell_ui/Docking/Widgets/DockHeaderWidget.h>

namespace rendell_ui {
DockHeaderWidget::DockHeaderWidget()
    : RectangleWidget() {
    _name = "DockHeaderWidget";

    _textWidget = createUnregisteredTextWidget();
    _textWidget->setAnchor(Anchor::leftStretch);
    _textWidget->setFontSize({20, 20});
    _textWidget->setInteract(false);
}
void DockHeaderWidget::draw() const {
    RectangleWidget::draw();
    _textWidget->draw();
}

void DockHeaderWidget::onMouseDown(glm::dvec2 cursorPosition) {
}

void DockHeaderWidget::onFreed(glm::dvec2 cursorPosition) {
    setOffset({0.0f, 0.0f});
}

void DockHeaderWidget::onDragged(glm::dvec2 startPoint, glm::dvec2 endPoint) {
    const glm::vec2 delta = static_cast<glm::vec2>(endPoint - startPoint);

    setOffset({0.0f, 0.0f});
    if (!intersect(endPoint)) {
        headerDragged.emit();
        return;
    }
    setOffset({static_cast<float>(endPoint.x - startPoint.x), _offset.y});
}

void DockHeaderWidget::setHeader(const std::string &header) {
    _textWidget->setText(std::wstring(header.begin(), header.end()));
}

void DockHeaderWidget::onSelfWeakPtrChanged() {
    _textWidget->setParent(_selfWeakPtr);
}
} // namespace rendell_ui