#include <algorithm>
#include <logging.h>
#include <rendell_ui/Widgets/AnchorableWidget.h>

namespace rendell_ui {
const Transform2D &AnchorableWidget::getTransform() const {
    return _transform;
}

glm::vec2 AnchorableWidget::getOffset() const {
    return _offset;
}

glm::vec2 AnchorableWidget::getSize() const {
    return _size;
}

Margins AnchorableWidget::getMargins() const {
    return _margins;
}

Anchor AnchorableWidget::getAnchor() const {
    return _anchor;
}

void AnchorableWidget::setOffset(glm::vec2 value) {
    if (_offset != value) {
        _offset = value;
        onOffsetChanged();
        offsetChanged.emit(_offset);
        updateMarkup();
    }
}

void AnchorableWidget::setSize(glm::vec2 value) {
    if (setSizePrivate(value)) {
        updateMarkup();
    }
}

void AnchorableWidget::setMargins(Margins value) {
    if (_margins != value) {
        _margins = value;
        onMarginsChanged();
        marginsChanged.emit(_margins);
        updateMarkup();
    }
}

void AnchorableWidget::setMargins(float left, float right, float bottom, float top) {
    setMargins({left, right, bottom, top});
}

void AnchorableWidget::setAnchor(Anchor value) {
    if (_anchor != value) {
        _anchor = value;
        onAnchorChanged();
        anchorChanged.emit(_anchor);
        updateMarkup();
    }
}

bool AnchorableWidget::intersect(glm::vec2 point) const {
    const glm::vec2 halfSize = _size * 0.5f * _transform.getScale();
    const glm::vec2 pos = _transform.getPosition();
    return std::clamp(point.x, pos.x - halfSize.x, pos.x + halfSize.x) == point.x &&
           std::clamp(point.y, pos.y - halfSize.y, pos.y + halfSize.y) == point.y;
}

bool AnchorableWidget::setSizePrivate(glm::vec2 value) {
    if (_size != value) {
        _size = value;
        onSizeChanged();
        sizeChanged.emit(value);
        return true;
    }
    return false;
}

void AnchorableWidget::updateMarkup() {
    const glm::vec2 parentPosition = getRootPosition();
    const glm::vec2 parentSize = getRootSize();
    const glm::vec2 halfParentSize = parentSize * 0.5f;

    switch (_anchor) {
    case Anchor::left: {
        _transform.setPosition(parentPosition - glm::vec2(halfParentSize.x, 0.0f) +
                               glm::vec2(_margins.left, 0.0f) + _size * glm::vec2(0.5f, 0.0f) +
                               _offset);
        break;
    }
    case Anchor::right: {
        _transform.setPosition(parentPosition + glm::vec2(halfParentSize.x, 0.0f) +
                               glm::vec2(-_margins.right, 0.0f) + _size * glm::vec2(-0.5f, 0.0f) +
                               _offset);
        break;
    }
    case Anchor::bottom: {
        _transform.setPosition(parentPosition + glm::vec2(0.0f, -halfParentSize.y) +
                               glm::vec2(0.0f, _margins.bottom) + _size * glm::vec2(0.0f, 0.5f) +
                               _offset);
        break;
    }
    case Anchor::top: {
        _transform.setPosition(parentPosition + glm::vec2(0.0f, halfParentSize.y) +
                               glm::vec2(0.0f, -_margins.top) + _size * glm::vec2(0.0f, -0.5f) +
                               _offset);
        break;
    }
    case Anchor::center: {
        _transform.setPosition(parentPosition + _offset);
        break;
    }
    case Anchor::leftBottom: {
        _transform.setPosition(parentPosition + glm::vec2(-halfParentSize.x, -halfParentSize.y) +
                               glm::vec2(_margins.left, _margins.bottom) +
                               _size * glm::vec2(0.5f, 0.5f) + _offset);
        break;
    }
    case Anchor::leftTop: {
        _transform.setPosition(parentPosition + glm::vec2(-halfParentSize.x, halfParentSize.y) +
                               glm::vec2(_margins.left, -_margins.top) +
                               _size * glm::vec2(0.5f, -0.5f) + _offset);
        break;
    }
    case Anchor::rightBottom: {
        _transform.setPosition(parentPosition + glm::vec2(halfParentSize.x, -halfParentSize.y) +
                               glm::vec2(-_margins.right, _margins.bottom) -
                               _size * glm::vec2(0.5f, -0.5f) + _offset);
        break;
    }
    case Anchor::rightTop: {
        _transform.setPosition(parentPosition + glm::vec2(halfParentSize.x, halfParentSize.y) +
                               glm::vec2(-_margins.right, -_margins.top) +
                               _size * glm::vec2(-0.5f, -0.5f) + _offset);
        break;
    }
    case Anchor::topStretch: {
        _transform.setPosition(parentPosition + glm::vec2(0.0f, halfParentSize.y) +
                               glm::vec2((_margins.left - _margins.right) * 0.5f, 0.0f) -
                               glm::vec2(0.0f, _size.y * 0.5f) + _offset);
        const float sizeX = parentSize.x - _margins.left - _margins.right;
        setSizePrivate({sizeX, _size.y});
        break;
    }
    case Anchor::horizontalStretch: {
        _transform.setPosition(parentPosition +
                               glm::vec2((_margins.left - _margins.right) * 0.5f, 0.0f) + _offset);
        const float sizeX = parentSize.x - _margins.left - _margins.right;
        setSizePrivate({sizeX, _size.y});
        break;
    }
    case Anchor::bottomStretch: {
        _transform.setPosition(parentPosition + glm::vec2(0.0f, -halfParentSize.y) +
                               glm::vec2((_margins.left - _margins.right) * 0.5f, _margins.bottom) +
                               glm::vec2(0.0f, _size.y * 0.5f) + _offset);
        const float sizeX = parentSize.x - _margins.left - _margins.right;
        setSizePrivate({sizeX, _size.y});
        break;
    }
    case Anchor::leftStretch: {
        _transform.setPosition(parentPosition + glm::vec2(-halfParentSize.x, 0.0f) +
                               glm::vec2(_margins.left, (_margins.bottom - _margins.top) * 0.5f) +
                               glm::vec2(_size.x * 0.5f, 0.0f) + _offset);
        const float sizeY = parentSize.y - _margins.bottom - _margins.top;
        setSizePrivate({_size.x, sizeY});
        break;
    }
    case Anchor::verticalStretch: {
        _transform.setPosition(parentPosition +
                               glm::vec2(0.0f, (_margins.bottom - _margins.top) * 0.5f) + _offset);
        const float sizeY = parentSize.y - _margins.bottom - _margins.top;
        setSizePrivate({_size.x, sizeY});
        break;
    }
    case Anchor::rightStretch: {
        _transform.setPosition(parentPosition + glm::vec2(halfParentSize.x, 0.0f) +
                               glm::vec2(-_margins.right, (_margins.bottom - _margins.top) * 0.5f) +
                               glm::vec2(-_size.x * 0.5f, 0.0f) + _offset);
        const float sizeY = parentSize.y - _margins.bottom - _margins.top;
        setSizePrivate({_size.x, sizeY});
        break;
    }
    case Anchor::centerStretch: {
        _transform.setPosition(
            parentPosition +
            glm::vec2((_margins.left - _margins.right), (_margins.bottom - _margins.top)) * 0.5f +
            _offset);
        const float sizeX = parentSize.x - _margins.left - _margins.right;
        const float sizeY = parentSize.y - _margins.bottom - _margins.top;
        setSizePrivate({sizeX, sizeY});
        break;
    }
    default: {
        const uint8_t anchorLogValue = static_cast<uint8_t>(_anchor);
        RUI_WARNING("The anchor {} is not implemented", static_cast<int>(anchorLogValue));
        return;
    }
    }

    onMarkupUpdated();
}

} // namespace rendell_ui
