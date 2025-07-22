#include <rendell_ui/Docking/DockNode.h>

namespace rendell_ui {
DockNode::DockNode(DockNodeSharedPtr parent, WidgetSharedPtr rootWidget)
    : _parent(parent)
    , _rootWidget(rootWidget) {
}

glm::vec2 DockNode::getOrigin() const {
    return _origin;
}

glm::vec2 DockNode::getSize() const {
    return _size;
}

void DockNode::setOrigin(glm::vec2 value) {
    if (_origin != value) {
        _origin = value;
        updateWidget();
    }
}

void DockNode::setSize(glm::vec2 value) {
    if (_size != value) {
        _size = value;
        updateWidget();
    }
}

void DockNode::setRect(glm::vec2 origin, glm::vec2 size) {
    if (_origin != origin || _size != size) {
        _origin = origin;
        _size = size;
        updateWidget();
    }
}

void DockNode::updateWidget() {
    _rootWidget->setAnchor(Anchor::center);
    _rootWidget->setSize(_size);
    const glm::vec2 offset = _origin + _size * 0.5f;
    _rootWidget->setOffset(offset);
}

} // namespace rendell_ui