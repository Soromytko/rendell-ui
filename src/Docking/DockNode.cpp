#include <algorithm>
#include <logging.h>
#include <rendell_ui/Docking/DockNode.h>
#include <rendell_ui/color_defines.h>

namespace rendell_ui {

DockNode::DockNode(DockNodeSharedPtr parent)
    : _parent(parent) {
}

float DockNode::getRatio() const {
    return _ratio;
}

bool DockNode::getIsHorizontal() const {
    return _isHorizontal;
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

DockNodeSharedPtr DockNode::getParent() const {
    return _parent.lock();
}

DockNodeSharedPtr DockNode::getFirstChild() const {
    return _firstChild;
}

DockNodeSharedPtr DockNode::getSecondChild() const {
    return _secondChild;
}

IDockNodeSeparatorSharedPtr DockNode::getSeparator() const {
    return _separator;
}

DockNode::RequestParentResizeCallback DockNode::getRequestParentResizeCallback() const {
    return _requestParentResizeCallback;
}

const std::vector<IDockableSharedPtr> &DockNode::getDockables() const {
    return _dockables;
}

bool DockNode::hasChildren() const {
    return _firstChild && _secondChild;
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

void DockNode::setRect(glm::vec2 origin, glm::vec2 size) {
    const glm::vec2 normalizedSize = glm::max(size, _minSize);
    if (_origin != origin || _size != normalizedSize) {
        _origin = origin;
        _size = normalizedSize;
        update();
    }
}

void DockNode::setChildren(DockNodeSharedPtr firstChild, DockNodeSharedPtr secondChild) {
    const auto setChild = [this](DockNodeSharedPtr &memberChild, DockNodeSharedPtr newChild) {
        if (memberChild != newChild) {
            if (memberChild) {
                memberChild->_parent.reset();
            }
            memberChild = newChild;
            if (memberChild) {
                memberChild->_parent = weak_from_this();
                memberChild->_depth = _depth + 1;
            }
        }
    };

    setChild(_firstChild, firstChild);
    setChild(_secondChild, secondChild);

    if (hasChildren()) {
        updateMinSizeAccordingChildren();
        updateChildren();
    }
}

void DockNode::setSeparator(IDockNodeSeparatorSharedPtr separator) {
    if (_separator != separator) {
        _separator = separator;
        if (_separator) {
            _separator->setOnDraggedCallback([&](float ratio) { setRatio(ratio); });
            _separator->setIsHorizontal(_isHorizontal);
            _separator->setRatio(_ratio);
        }
    }
}

void DockNode::setRequestParentResizeCallback(RequestParentResizeCallback callback) {
    _requestParentResizeCallback = callback;
}

void DockNode::setRatio(float value) {
    const float newRatio = normalizeRatio(value);
    if (_ratio != newRatio) {
        _ratio = newRatio;
        if (_separator) {
            _separator->setRatio(_ratio);
        }
        if (hasChildren()) {
            updateChildren();
        }
    }
}

void DockNode::setIsHorizontal(bool value) {
    if (_isHorizontal != value) {
        _isHorizontal = value;
        if (_separator) {
            _separator->setIsHorizontal(_isHorizontal);
        }
    }
}

void DockNode::setDockables(std::vector<IDockableSharedPtr> dockables) {
    if (_dockables != dockables || true) {
        _dockables = std::move(dockables);

        const DockNodeSharedPtr self = shared_from_this();
        for (const IDockableSharedPtr dockable : _dockables) {
            dockable->setMinSizeChangedCallback(
                [this](glm::vec2 minSize) { updateMinSizeAccordingDockables(); });

            IDockableWeakPtr dockableWeak = dockable;
            dockable->setHeaderDraggedCallback([this, dockableWeak]() {
                const auto lockedDockable = dockableWeak.lock();
                if (!lockedDockable) {
                    return;
                }

                auto it = std::find(_dockables.begin(), _dockables.end(), lockedDockable);
                if (it == _dockables.end()) {
                    return;
                }

                _dockables.erase(it);
                if (!_dockables.empty()) {
                    return;
                }

                if (auto lockedParent = _parent.lock()) {
                    const DockNodeSharedPtr child = lockedParent->_firstChild.get() == this
                                                        ? lockedParent->_secondChild
                                                        : lockedParent->_firstChild;
                    if (child->hasChildren()) {
                        const DockNodeSharedPtr self = shared_from_this();

                        lockedParent->setChildren(nullptr, nullptr);
                        lockedParent->_isHorizontal = child->_isHorizontal;
                        lockedParent->_ratio = child->_ratio;
                        lockedParent->_separator->release();
                        lockedParent->setSeparator(child->_separator);
                        lockedParent->setChildren(child->_firstChild, child->_secondChild);
                        lockedParent->updateSeparator();
                    } else {
                        lockedParent->_separator->release();
                        lockedParent->_separator = nullptr;
                        lockedParent->setChildren(nullptr, nullptr);
                        lockedParent->takeDockablesFrom(child);
                        lockedParent->update();
                    }
                }
            });
        }

        updateMinSizeAccordingDockables();
    }
}

void DockNode::moveDockablesTo(DockNodeSharedPtr dockNodeReceiver) {
    assert(dockNodeReceiver);
    assert(dockNodeReceiver.get() != this);

    dockNodeReceiver->setDockables(std::move(_dockables));
    _dockables.clear();
}

void DockNode::takeDockablesFrom(DockNodeSharedPtr dockNodeSender) {
    assert(dockNodeSender);
    assert(dockNodeSender.get() != this);

    setDockables(std::move(dockNodeSender->_dockables));
    dockNodeSender->_dockables.clear();
}

glm::vec2 DockNode::getCenter() const {
    return _origin + _size * 0.5f;
}

glm::vec2 DockNode::getDeltaSize(glm::vec2 newSize) const {
    return glm::max(newSize, _minSize) - glm::max(_size, _minSize);
}

float DockNode::normalizeRatio(float ratio) const {
    if (!hasChildren()) {
        return std::clamp(ratio, 0.0f, 1.0f);
    }
    assert(_firstChild);
    assert(_secondChild);
    assert(_separator);

    const auto [minRatio, maxRatio] =
        _isHorizontal
            ? std::pair((_firstChild->_minSize.y) / _size.y,
                        (_size.y - _separator->getThickness() - _secondChild->_minSize.y) / _size.y)
            : std::pair((_firstChild->_minSize.x / _size.x),
                        (_size.x - _separator->getThickness() - _secondChild->_minSize.x) /
                            _size.x);

    return std::clamp(std::min(std::max(minRatio, ratio), maxRatio), 0.0f, 1.0f);
}

void DockNode::setMinSize(glm::vec2 value) {
    if (_minSize != value) {
        _minSize = value;
        setSize(glm::max(_minSize, _size));
        if (auto lockedParent = _parent.lock()) {
            lockedParent->updateMinSizeAccordingChildren();
        } else if (_requestParentResizeCallback) {
            _requestParentResizeCallback(_minSize);
        }
    }
}

void DockNode::updateMinSizeAccordingChildren() {
    assert(_firstChild);
    assert(_secondChild);

    glm::vec2 newMinSize;
    if (_isHorizontal) {
        newMinSize = glm::vec2(std::max(_firstChild->_minSize.x, _secondChild->_minSize.x),
                               _firstChild->_minSize.y + _secondChild->_minSize.y +
                                   _separator->getThickness());
    } else {
        newMinSize = glm::vec2(_firstChild->_minSize.x + _secondChild->_minSize.x +
                                   _separator->getThickness(),
                               std::max(_firstChild->_minSize.y, _secondChild->_minSize.y));
    }

    setMinSize(newMinSize);
}

void DockNode::updateMinSizeAccordingDockables() {
    if (!_dockables.empty()) {
        glm::vec2 newMinSize = _dockables[0]->getMinSize();
        for (const IDockableSharedPtr dockable : _dockables) {
            newMinSize = glm::max(newMinSize, dockable->getMinSize());
        }
        setMinSize(newMinSize);
    }
}

void DockNode::updateChildren() {
    assert(_firstChild);
    assert(_secondChild);

    const float halfSeparatorThickness = _separator->getThickness() * 0.5f;

    if (_isHorizontal) {
        _firstChild->setRect(_origin,
                             glm::vec2(_size.x, _size.y * _ratio - halfSeparatorThickness));
        _secondChild->setRect(
            _origin + glm::vec2(0.0f, _size.y * _ratio + halfSeparatorThickness),
            glm::vec2(_size.x, _size.y * (1.0f - _ratio) - halfSeparatorThickness));

    } else {
        _firstChild->setRect(_origin,
                             glm::vec2(_size.x * _ratio - halfSeparatorThickness, _size.y));
        _secondChild->setRect(
            _origin + glm::vec2(_size.x * _ratio + halfSeparatorThickness, 0.0f),
            glm::vec2(_size.x * (1.0f - _ratio) - halfSeparatorThickness, _size.y));
    }

    _isChildrenUpdated = true;
}

void DockNode::update() {
    if (!hasChildren()) {
        updateDockables();
        return;
    }
    assert(_firstChild);
    assert(_secondChild);
    assert(_separator);

    updateSeparator();

    _isChildrenUpdated = false;
    updateRatio();

    // The updateRatio method could already update children.
    if (!_isChildrenUpdated) {
        updateChildren();
    }
}

void DockNode::updateDockables() {
    for (IDockableSharedPtr dockable : _dockables) {
        dockable->setSize(_size);
        dockable->setPosition(getCenter());
    }
}

void DockNode::updateRatio() {
    setRatio(_ratio);
}

void DockNode::updateSeparator() {
    assert(_separator);
    _separator->setDockNodeRect(getCenter(), _size);
    _separator->setRatio(_ratio);
}

} // namespace rendell_ui