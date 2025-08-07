#include <cassert>
#include <rendell_ui/Docking/DockTreeBuilder.h>

namespace rendell_ui {

DockTreeBuilder::DockTreeBuilder(WidgetSharedPtr rootWidget)
    : _tree(makeDockTree()) {
    assert(_tree);
    _curretDockNode = _tree->createRootNode(rootWidget);
}

DockTreeBuilder &DockTreeBuilder::setWindow(IWindowWeakPtr window) {
    assert(_tree);
    _tree->setWindow(window);
    return *this;
}

DockTreeBuilder &
DockTreeBuilder::setCreateSeparatorDelegate(DockTree::CreateSeparatorDelegate delegate) {
    assert(_tree);
    _tree->setCreateSeparatorDelegate(delegate);
    return *this;
}

DockTreeBuilder &DockTreeBuilder::splitHorizontal(float ratio) {
    assert(_curretDockNode);
    _tree->split(_curretDockNode, true, ratio);
    return *this;
}

DockTreeBuilder &DockTreeBuilder::splitVertical(float ratio) {
    assert(_curretDockNode);
    _tree->split(_curretDockNode, false, ratio);
    return *this;
}

DockTreeBuilder &DockTreeBuilder::goFirst() {
    _curretDockNode = _curretDockNode->getFirstChild();
    assert(_curretDockNode);
    return *this;
}

DockTreeBuilder &DockTreeBuilder::goSecond() {
    _curretDockNode = _curretDockNode->getSecondChild();
    assert(_curretDockNode);
    return *this;
}

DockTreeBuilder &DockTreeBuilder::goParent() {
    _curretDockNode = _curretDockNode->getParent();
    assert(_curretDockNode);
    return *this;
}

DockTreeBuilder &DockTreeBuilder::goRoot() {
    assert(_tree);
    _curretDockNode = _tree->getRootNode();
    assert(_curretDockNode);
    return *this;
}

DockTreeBuilder &DockTreeBuilder::setDockables(std::vector<IDockableSharedPtr> dockables) {
    assert(_curretDockNode);
    _curretDockNode->setDockables(std::move(dockables));
    return *this;
}

DockTreeBuilder &DockTreeBuilder::setDockable(IDockableSharedPtr dockables) {
    assert(_tree);
    assert(_curretDockNode);
    _curretDockNode->setDockables({dockables});
    //_tree->addDockable(dockables);
    return *this;
}

DockTreeSharedPtr DockTreeBuilder::build() {
    assert(_tree);
    return _tree;
}

} // namespace rendell_ui