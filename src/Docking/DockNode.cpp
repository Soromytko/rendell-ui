#include <logging.h>
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

WidgetSharedPtr DockNode::getRootWidget() const {
    return _rootWidget;
}

DockNodeSharedPtr DockNode::getParent() const {
    return _parent.lock();
}

DockNodeSharedPtr DockNode::getFirstChild() const {
    return _firstChild;
}

DockNodeSharedPtr DockNode::getSecondChild() const {
    return _secondChild;
}

bool DockNode::hasChildren() const {
    return _firstChild || _secondChild;
}

void DockNode::setOrigin(glm::vec2 value) {
    if (_origin != value) {
        _origin = value;
        update();
    }
}

void DockNode::setSize(glm::vec2 value) {
    if (_size != value) {
        _size = value;
        update();
    }
}

void DockNode::setRect(glm::vec2 origin, glm::vec2 size) {
    if (_origin != origin || _size != size) {
        _origin = origin;
        _size = size;
        update();
    }
}

void DockNode::setChildren(DockNodeSharedPtr firstChild, DockNodeSharedPtr secondChild) {
    _firstChild = firstChild;
    _secondChild = secondChild;
}

void DockNode::setFirstChild(DockNodeSharedPtr firstChild) {
    _firstChild = firstChild;
}

void DockNode::setSecondChild(DockNodeSharedPtr secondChild) {
    _secondChild = secondChild;
}

bool DockNode::splitBy(DockNodeSharedPtr dockNode, SplitType type) {
    if (hasChildren()) {
        RUI_WARNING("This node has already been split");
        return false;
    }

    _ratio = 0.7f;

    DockNodeSharedPtr surrogateDockNode = makeDockNode(shared_from_this(), _rootWidget);

    if (type == SplitType::right || type == SplitType::bottom) {
        _firstChild = surrogateDockNode;
        _secondChild = dockNode;
    } else {
        _firstChild = dockNode;
        _secondChild = surrogateDockNode;
    }

    update();

    return true;
}

bool DockNode::release() {
    if (auto lockedParent = _parent.lock()) {
        if (lockedParent->_firstChild.get() == this) {
            lockedParent->replaceWith(lockedParent->_secondChild);
        } else {
            assert(lockedParent->_secondChild.get() == this);
            lockedParent->replaceWith(lockedParent->_firstChild);
        }
        return true;
    }

    return false;
}

void DockNode::replaceWith(DockNodeSharedPtr dockNode) {
    _rootWidget = dockNode->_rootWidget;
    _firstChild = _secondChild = nullptr;
}

void DockNode::updateWidget() {
    _rootWidget->setAnchor(Anchor::center);
    _rootWidget->setSize(_size);
    const glm::vec2 offset = _origin + _size * 0.5f;
    _rootWidget->setOffset(offset);
}

void DockNode::update() {
    if (_rootWidget && !hasChildren()) {
        updateWidget();
        return;
    }

    if (_firstChild) {
        _firstChild->setRect(_origin, glm::vec2(_size.x * _ratio, _size.y));
    }
    if (_secondChild) {
        const float reverseRatio = 1.0f - _ratio;
        _secondChild->setRect(_origin + glm::vec2(_size.x * _ratio, 0.0f),
                              glm::vec2(_size.x * reverseRatio, _size.y));
    }
}



//void splitDockNode(DockNodeSharedPtr node, WidgetSharedPtr rootWidget) {
//    if (node->hasChildren()) {
//        return;
//    }
//
//    DockNodeSharedPtr firstNode = makeDockNode(node, node->getRootWidget());
//    DockNodeSharedPtr secondNode = makeDockNode(node, rootWidget);
//
//    node->setChildren(firstNode, secondNode);
//}

} // namespace rendell_ui