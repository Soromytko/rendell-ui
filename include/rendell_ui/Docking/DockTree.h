#pragma once
#include "DockNode.h"
#include <rendell_ui/Widgets/Widget.h>
#include <rendell_ui/defines.h>
#include <rendell_ui/Window/IWindow.h>

namespace rendell_ui {
class DockTree : std::enable_shared_from_this<DockTree> {
public:
    using CreateSeparatorDelegate =
        std::function<DockSeparatorWidgetSharedPtr(WidgetSharedPtr rootWidget)>;

    DockTree(WidgetSharedPtr rootWidget);
    virtual ~DockTree() = default;

    DockNodeSharedPtr getRootNode() const;
    IWindowWeakPtr getWindow() const;
    CreateSeparatorDelegate getCreateSeparatorDelegate() const;

    void setWindow(IWindowWeakPtr window);
    void setCreateSeparatorDelegate(CreateSeparatorDelegate delegate); 

    bool split(DockNodeSharedPtr dockNode, bool isHorizontal, float ratio = 0.5f);

private:
    DockSeparatorWidgetSharedPtr createSeparator() const;

    CreateSeparatorDelegate _createSeparatorDelegate;
    std::unordered_map<DockNodeSharedPtr, DockSeparatorWidgetSharedPtr> _separators;

    WidgetSharedPtr _rootWidget;
    DockNodeSharedPtr _rootDockNode;
    IWindowWeakPtr _window;
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(DockTree)

} // namespace rendell_ui