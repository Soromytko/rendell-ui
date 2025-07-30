#pragma once
#include "DockNode.h"
#include <rendell_ui/Widgets/Widget.h>
#include <rendell_ui/Window/IWindow.h>
#include <rendell_ui/defines.h>

namespace rendell_ui {
class DockTree : public std::enable_shared_from_this<DockTree> {
public:
    using CreateSeparatorDelegate =
        std::function<DockSeparatorWidgetSharedPtr(WidgetSharedPtr rootWidget)>;

    DockTree();
    virtual ~DockTree() = default;

    DockNodeSharedPtr getRootNode() const;
    IWindow *getWindow() const;
    CreateSeparatorDelegate getCreateSeparatorDelegate() const;

    void setWindow(IWindow *window);
    void setCreateSeparatorDelegate(CreateSeparatorDelegate delegate);

    DockNodeSharedPtr createRootNode(WidgetSharedPtr rootWidget);
    bool split(DockNodeSharedPtr dockNode, bool isHorizontal, float ratio = 0.5f);

private:
    DockSeparatorWidgetSharedPtr createSeparator() const;

    CreateSeparatorDelegate _createSeparatorDelegate{};
    std::unordered_map<DockNodeSharedPtr, DockSeparatorWidgetSharedPtr> _separators{};

    WidgetSharedPtr _rootWidget{nullptr};
    DockNodeSharedPtr _rootDockNode{nullptr};
    IWindow *_window{nullptr};
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(DockTree)

} // namespace rendell_ui