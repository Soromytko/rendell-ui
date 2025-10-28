#include <rendell_ui/Docking/DockTree.h>

#include <rendell_ui/rendell_ui.h>

namespace rendell_ui {
DockTree::DockTree() {
}

DockNodeSharedPtr DockTree::getRootNode() const {
    return _rootDockNode;
}

IWindow *DockTree::getWindow() const {
    return _window;
}

DockTree::CreateSeparatorDelegate DockTree::getCreateSeparatorDelegate() const {
    return _createSeparatorDelegate;
}

void DockTree::setWindow(IWindow *window) {
    _window = window;
}

void DockTree::setCreateSeparatorDelegate(CreateSeparatorDelegate delegate) {
    _createSeparatorDelegate = delegate;
}

DockNodeSharedPtr DockTree::createRootNode(WidgetSharedPtr rootWidget) {
    _rootWidget = rootWidget;

    _rootDockNode = makeDockNode(nullptr);
    _rootDockNode->setRequestParentResizeCallback([thisWeak = weak_from_this()](glm::vec2 size) {
        const DockTreeSharedPtr tree = thisWeak.lock();
        if (!tree) {
            RUI_WARNING("The captured object \"DockTree\" could have been destroyed!");
            return;
        }

        IWindow *window = tree->_window;
        if (!isValidWindow(window)) {
            RUI_DEBUG("The Window of DockNode is nullptr!");
            return;
        }

        const glm::ivec2 newWindowSize =
            glm::max(static_cast<glm::ivec2>(size), window->getMinSize());
        RUI_INFO("{} {}", newWindowSize.x, newWindowSize.y);
        window->setMinSize((newWindowSize));
    });
    _rootDockNode->setSize(_rootWidget->getSize());

    return _rootDockNode;
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
