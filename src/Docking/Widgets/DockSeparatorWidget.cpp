#include <rendell_ui/Docking/Widgets/DockSeparatorWidget.h>

#include <algorithm>

namespace rendell_ui {
DockSeparatorWidget::DockSeparatorWidget()
    : RectangleWidget() {
    setName("DockSeparatorWidget");
}

void DockSeparatorWidget::onMouseDown(glm::dvec2 cursorPosition) {
    _startRatio = _ratio;
}

void DockSeparatorWidget::onMouseHovered(glm::dvec2 cursorPosition) {
    const auto cursorType =
        _isHorizontal ? WindowCursorType::verticalResize : WindowCursorType::horizontalResize;
    setWindowCursorType(cursorType);
}

void DockSeparatorWidget::onMouseExited() {
    setWindowCursorType(WindowCursorType::arrow);
}

void DockSeparatorWidget::onDragged(glm::dvec2 startPoint, glm::dvec2 endPoint) {
    if (!_onDraggedCallback) {
        return;
    }

    const float startPointDim = static_cast<float>(_isHorizontal ? startPoint.y : startPoint.x);
    const float endPointDim = static_cast<float>(_isHorizontal ? endPoint.y : endPoint.x);
    const float sizeSim = _isHorizontal ? _dockNodeSize.y : _dockNodeSize.x;

    const float dragDelta = endPointDim - startPointDim;
    if (dragDelta == 0.0f) {
        return;
    }

    const float newRatio = _startRatio + dragDelta / sizeSim;
    _onDraggedCallback(newRatio);
}

bool DockSeparatorWidget::getIsHorizontal() const {
    return _isHorizontal;
}

float DockSeparatorWidget::getRatio() const {
    return _ratio;
}

float DockSeparatorWidget::getThickness() const {
    return _thickness;
}

void DockSeparatorWidget::setIsHorizontal(bool value) {
    if (_isHorizontal != value) {
        _isHorizontal = value;
        update();
    }
}

void DockSeparatorWidget::setRatio(float value) {
    const float ratio = std::clamp(value, 0.0f, 1.0f);
    if (_ratio != ratio) {
        _ratio = ratio;
        update();
    }
}

void DockSeparatorWidget::setThickness(float value) {
    if (_thickness != value) {
        _thickness = value;
        update();
    }
}

void DockSeparatorWidget::setDockNodeRect(glm::vec2 dockNodeCenter, glm::vec2 dockNodeSize) {
    if (_dockNodeCenter != dockNodeCenter || _dockNodeSize != dockNodeSize) {
        _dockNodeCenter = dockNodeCenter;
        _dockNodeSize = dockNodeSize;
        // TODO: Can we call only one update?
        update();
        updateMarkup();
    }
}

void DockSeparatorWidget::setOnDraggedCallback(OnDraggedCallback callback) {
    _onDraggedCallback = callback;
}

void DockSeparatorWidget::release() {
    setParent({});
}

glm::vec2 DockSeparatorWidget::getRootPosition() const {
    return _dockNodeCenter;
}

glm::vec2 DockSeparatorWidget::getRootSize() const {
    return _dockNodeSize;
}

float DockSeparatorWidget::getRatioWithDelta(float delta) const {
    const float size = _isHorizontal ? _dockNodeSize.y : _dockNodeSize.x;
    return std::clamp(delta / size, 0.0f, 1.0f);
}

void DockSeparatorWidget::update() {
    if (_isHorizontal) {
        setAnchor(Anchor::bottom);
        setSize({_dockNodeSize.x, _thickness});
        setOffset(glm::vec2(0.0f, _dockNodeSize.y * _ratio - _thickness * 0.5f));
    } else {
        setAnchor(Anchor::left);
        setSize({_thickness, _dockNodeSize.y});
        setOffset(glm::vec2(_dockNodeSize.x * _ratio - _thickness * 0.5f, 0.0f));
    }
}

} // namespace rendell_ui