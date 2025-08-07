#pragma once
#include "DockNode.h"
#include "DockTree.h"
#include "IDockable.h"
#include <rendell_ui/Widgets/Widget.h>
#include <rendell_ui/Window/IWindow.h>
#include <vector>

namespace rendell_ui {
class DockTreeBuilder final {
public:
    DockTreeBuilder(WidgetSharedPtr rootWidget);
    ~DockTreeBuilder() = default;

    DockTreeBuilder &setWindow(IWindowWeakPtr window);
    DockTreeBuilder &setCreateSeparatorDelegate(DockTree::CreateSeparatorDelegate delegate);
    DockTreeBuilder &splitHorizontal(float ratio = 0.5f);
    DockTreeBuilder &splitVertical(float ratio = 0.5f);
    DockTreeBuilder &goFirst();
    DockTreeBuilder &goSecond();
    DockTreeBuilder &goParent();
    DockTreeBuilder &goRoot();
    DockTreeBuilder &setDockables(std::vector<IDockableSharedPtr> dockables);
    DockTreeBuilder &setDockable(IDockableSharedPtr dockable);

    DockTreeSharedPtr build();

private:
    DockTreeSharedPtr _tree;
    DockNodeSharedPtr _curretDockNode{nullptr};
};
} // namespace rendell_ui