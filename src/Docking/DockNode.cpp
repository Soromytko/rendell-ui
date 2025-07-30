#include <algorithm>
#include <logging.h>
#include <rendell_ui/Docking/DockNode.h>
#include <rendell_ui/color_defines.h>
#include <tuple>

namespace rendell_ui {
DockNode::CreateDockSeparatorCallback s_createDockSeparatorCallback = [](WidgetSharedPtr parent) {
    return createDockSeparatorWidget(parent);
};
} // namespace rendell_ui

namespace rendell_ui {
void DockNode::setCreateDockSeparatorCallback(CreateDockSeparatorCallback callback) {
    assert(callback != nullptr);
    s_createDockSeparatorCallback = callback;
}

DockNode::DockNode(DockNodeSharedPtr parent, WidgetSharedPtr canvasRootWidget)
    : _parent(parent)
    , _canvasRootWidget(canvasRootWidget) {
    _rootWidget = createRootWidget();
}

const std::string &DockNode::getHeader() const {
    return _header;
}

glm::vec2 DockNode::getOrigin() const {
    return _origin;
}

glm::vec2 DockNode::getSize() const {
    return _size;
}

glm::vec2 DockNode::getMinSize() const {
    return _minSize;
}

DockRootWidgetSharedPtr DockNode::getRootWidget() const {
    return _rootWidget;
}

WidgetSharedPtr DockNode::getWorkSpaceWidget() const {
    return _rootWidget->getWorkSpaceWidget();
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

DockSeparatorWidgetSharedPtr DockNode::getSeparator() const {
    return _dockSeparatorWidget;
}

bool DockNode::hasChildren() const {
    return _firstChild || _secondChild;
}

void DockNode::setHeader(const std::string &header) {
    if (_header != header) {
        _rootWidget->getTitleBarWidget()->getHeaderWidget()->setHeader(header);
        _header = header;
    }
}

void DockNode::setOrigin(glm::vec2 value) {
    if (_origin != value) {
        _origin = value;
        update();
    }
}

void DockNode::setSize(glm::vec2 value) {
    const glm::vec2 normalizedSize = glm::max(_minSize, value);
    if (_size != normalizedSize) {
        _size = normalizedSize;
        update();
    }
}

void DockNode::setMinSize(glm::vec2 value) {
    _minSize = value;
}

void DockNode::setRect(glm::vec2 origin, glm::vec2 size) {
    const glm::vec2 normalizedSize = glm::max(size, _minSize);
    if (_origin != origin || _size != normalizedSize) {
        _origin = origin;
        _size = normalizedSize;
        update();
    }
}

bool DockNode::splitBy(DockNodeSharedPtr dockNode, SplitType type) {
    if (hasChildren()) {
        RUI_WARNING("This node has already been split");
        return false;
    }

    DockNodeSharedPtr surrogateDockNode = makeDockNode(shared_from_this(), _canvasRootWidget);
    surrogateDockNode->setRootWidget(_rootWidget);
    surrogateDockNode->setMinSize(_minSize);

    const bool isHorizontal = type == SplitType::bottom || type == SplitType::top;

    _firstChild = surrogateDockNode;
    _secondChild = dockNode;
    _firstChild->_depth = _secondChild->_depth = _depth + 1;

    _rootWidget = nullptr;

    // Separator
    _dockSeparatorWidget = s_createDockSeparatorCallback(_canvasRootWidget);
    _dockSeparatorWidget->setIsHorizontal(isHorizontal);
    _dockSeparatorWidget->setNormalizeRatioCallback(
        [&](float ratio) -> float { return getNormalizedRatio(ratio); });
    _dockSeparatorWidget->ratioChanged.connect(this, &DockNode::onRatioChanged);
    _dockSeparatorWidget->setDockNodeRect(_size, getCenter());
    _dockSeparatorWidget->setRatio(0.5f);

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

glm::vec2 DockNode::getCenter() const {
    return _origin + _size * 0.5f;
}

glm::vec2 DockNode::getDeltaSize(glm::vec2 newSize) const {
    return glm::max(newSize, _minSize) - glm::max(_size, _minSize);
}

DockRootWidgetSharedPtr DockNode::createRootWidget() const {
    DockRootWidgetSharedPtr result = createDockRootWidget(_canvasRootWidget);
    result->setAnchor(Anchor::center);
    return result;
}

float DockNode::getNormalizedRatio(float ratio) const {
    ratio = std::clamp(ratio, 0.0f, 1.0f);

    const float halfThickness = _dockSeparatorWidget->getThickness() * 0.5f;

    const auto [newFirstSize, newSecondSize, sizeDim] =
        _dockSeparatorWidget->getIsHorizontal()
            ? std::tuple(
                  glm::max(_firstChild->_minSize.y + halfThickness, _size.y * ratio),
                  glm::max(_secondChild->_minSize.y + halfThickness, _size.y * (1.0f - ratio)),
                  _size.y)
            : std::tuple(
                  glm::max(_firstChild->_minSize.x + halfThickness, _size.x * ratio),
                  glm::max(_secondChild->_minSize.x + halfThickness, _size.x * (1.0f - ratio)),
                  _size.x);

    return (newFirstSize <= newSecondSize) ? (newFirstSize / sizeDim)
                                           : ((sizeDim - newSecondSize) / sizeDim);
}

void DockNode::setRootWidget(DockRootWidgetSharedPtr widget) {
    rendell_ui::release_widget(_rootWidget);
    _rootWidget = widget;
}

void DockNode::replaceWith(DockNodeSharedPtr dockNode) {
    _rootWidget = dockNode->_rootWidget;
    _firstChild->_parent.reset();
    _secondChild->_parent.reset();
    _firstChild = _secondChild = nullptr;
}

void DockNode::onRatioChanged(float ratio) {
    assert(_dockSeparatorWidget);
    assert(_firstChild);
    assert(_secondChild);

    updateChildren(ratio);
}

void DockNode::updateRootWidget() {
    assert(_rootWidget);
    _rootWidget->setAnchor(Anchor::center);
    _rootWidget->setSize(_size);
    _rootWidget->setOffset(getCenter());
}

void DockNode::updateChildren(float ratio) {
    assert(_firstChild);
    assert(_secondChild);

    const float halfSeparatorThickness = _dockSeparatorWidget->getThickness() * 0.5f;

    if (_dockSeparatorWidget->getIsHorizontal()) {
        _firstChild->setRect(_origin, glm::vec2(_size.x, _size.y * ratio - halfSeparatorThickness));
        _secondChild->setRect(
            _origin + glm::vec2(0.0f, _size.y * ratio + halfSeparatorThickness),
            glm::vec2(_size.x, _size.y * (1.0f - ratio) - halfSeparatorThickness));

    } else {
        _firstChild->setRect(_origin, glm::vec2(_size.x * ratio - halfSeparatorThickness, _size.y));
        _secondChild->setRect(
            _origin + glm::vec2(_size.x * ratio + halfSeparatorThickness, 0.0f),
            glm::vec2(_size.x * (1.0f - ratio) - halfSeparatorThickness, _size.y));
    }

    _isChildrenUpdated = true;
}

void DockNode::update() {
    if (_rootWidget && !hasChildren()) {
        updateRootWidget();
        return;
    }
    assert(_firstChild);
    assert(_secondChild);
    assert(_dockSeparatorWidget);

    _isChildrenUpdated = false;
    _dockSeparatorWidget->setDockNodeRect(_size, getCenter());

    // The setDockNodeRect method could already update children.
    if (!_isChildrenUpdated) {
        updateChildren(_dockSeparatorWidget->getRatio());
    }
}

} // namespace rendell_ui