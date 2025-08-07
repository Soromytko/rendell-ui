#include <rendell_ui/Docking/DockTree.h>

namespace rendell_ui {

DockTree::DockTree(WidgetSharedPtr rootWidget)
    : _rootWidget(rootWidget) {
    _rootDockNode = makeDockNode(nullptr);
    _rootDockNode->setRequestParentResizeCallback([this](glm::vec2 size) {
        if (auto lockedWindow = _window.lock()) {
            const glm::ivec2 newWindowSize =
                glm::max(static_cast<glm::ivec2>(size), lockedWindow->getMinSize());
            RUI_INFO("{} {}", newWindowSize.x, newWindowSize.y);
            lockedWindow->setMinSize((newWindowSize));
        }
    });
    _rootDockNode->setSize(_rootWidget->getSize());
}

DockNodeSharedPtr DockTree::getRootNode() const {
    return _rootDockNode;
}

IWindowWeakPtr DockTree::getWindow() const {
    return _window;
}

DockTree::CreateSeparatorDelegate DockTree::getCreateSeparatorDelegate() const {
    return _createSeparatorDelegate;
}

void DockTree::setWindow(IWindowWeakPtr window) {
    _window = window;
}

void DockTree::setCreateSeparatorDelegate(CreateSeparatorDelegate delegate) {
    _createSeparatorDelegate = delegate;
}

bool DockTree::split(DockNodeSharedPtr dockNode, bool isHorizontal, float ratio) {
    if (dockNode->hasChildren()) {
        RUI_WARNING("This DockNode has already been split");
        return false;
    }

    DockNodeSharedPtr firstChild = makeDockNode(dockNode);
    DockNodeSharedPtr secondChild = makeDockNode(dockNode);
    firstChild->moveDockablesTo(dockNode);

    dockNode->setIsHorizontal(isHorizontal);
    dockNode->setSeparator(createSeparator());
    dockNode->setChildren(firstChild, secondChild);
    dockNode->setRatio(ratio);

    return true;
}

DockSeparatorWidgetSharedPtr DockTree::createSeparator() const {
    return _createSeparatorDelegate ? _createSeparatorDelegate(_rootWidget)
                                    : createDockSeparatorWidget(_rootWidget);
}

} // namespace rendell_ui
