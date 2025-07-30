#include <algorithm>
#include <rendell_ui/Docking/Widgets/DockSeparatorWidget.h>

namespace rendell_ui {
DockSeparatorWidget::DockSeparatorWidget()
    : RectangleWidget() {
    setName("DockSeparatorWidget");
}

void DockSeparatorWidget::onMouseDown(glm::dvec2 cursorPosition) {
    _startDragOffset = _dragOffset;
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
    if (!_normalizeRatioCallback) {
        return;
    }

    const float start = static_cast<float>(_isHorizontal ? startPoint.y : startPoint.x);
    const float end = static_cast<float>(_isHorizontal ? endPoint.y : endPoint.x);

    const float dragDelta = end - start;
    if (dragDelta == 0.0f) {
        return;
    }

    const float newRatio = getRatioWithDelta(_startDragOffset + dragDelta);
    const float normalizedRatio = _normalizeRatioCallback(newRatio);
    setRatio(normalizedRatio);
}

bool DockSeparatorWidget::getIsHorizontal() const {
    return _isHorizontal;
}

DockSeparatorWidget::NormalizeRatioCallback DockSeparatorWidget::getNormalizeRatioCallback() const {
    return _normalizeRatioCallback;
}

float DockSeparatorWidget::getThickness() const {
    return _thickness;
}

float DockSeparatorWidget::getRatio() const {
    return getRatioWithDelta(_dragOffset);
}

void DockSeparatorWidget::setThickness(float value) {
    if (_thickness != value) {
        _thickness = value;
        update();
    }
}

void DockSeparatorWidget::setIsHorizontal(bool value) {
    if (_isHorizontal != value) {
        _isHorizontal = value;
        update();
    }
}

void DockSeparatorWidget::setNormalizeRatioCallback(NormalizeRatioCallback callback) {
    _normalizeRatioCallback = callback;
}

void DockSeparatorWidget::setDockNodeRect(glm::vec2 size, glm::vec2 center) {
    if (_dockNodeSize != size || _dockNodeCenter != center) {
        _dockNodeSize = size;
        _dockNodeCenter = center;
        updateRatioWithNewSize();
        // TODO: Can we call only one update?
        update();
        updateMarkup();
    }
}

void DockSeparatorWidget::setRatio(float value) {
    const float ratio = std::clamp(value, 0.0f, 1.0f);
    setDragOffset(ratio * (_isHorizontal ? _dockNodeSize.y : _dockNodeSize.x));
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

void DockSeparatorWidget::setDragOffset(float value) {
    if (_dragOffset != value) {
        _dragOffset = value;
        update();
        ratioChanged.emit(getRatio());
    }
}

void DockSeparatorWidget::updateRatioWithNewSize() {
    if (_oldDockNodeSize == _dockNodeSize || !_normalizeRatioCallback) {
        return;
    }

    const float sizeDim = _isHorizontal ? _dockNodeSize.y : _dockNodeSize.x;
    const float oldSizeDim = _isHorizontal ? _oldDockNodeSize.y : _oldDockNodeSize.x;

    if (oldSizeDim > 0.0f) {
        const float scaleRatio = sizeDim / oldSizeDim;
        const float newRatio = _normalizeRatioCallback(getRatio() * scaleRatio);
        setRatio(newRatio);
    }

    _oldDockNodeSize = _dockNodeSize;
}

void DockSeparatorWidget::update() {
    if (_isHorizontal) {
        setAnchor(Anchor::bottom);
        setSize({_dockNodeSize.x, _thickness});
        setOffset(glm::vec2(0.0f, _dragOffset - _thickness * 0.5f));
    } else {
        setAnchor(Anchor::left);
        setSize({_thickness, _dockNodeSize.y});
        setOffset(glm::vec2(_dragOffset - _thickness * 0.5f, 0.0f));
    }
}

} // namespace rendell_ui