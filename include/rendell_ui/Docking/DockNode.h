#pragma once
#include "DockSeparator.h"
#include <rendell_ui/Widgets/Widget.h>
#include <rendell_ui/defines.h>

namespace rendell_ui {
class DockNode;

RENDELL_UI_DECLARE_SHARED_PTR(DockNode)
RENDELL_UI_DECLARE_WEAK_PTR(DockNode)

class DockNode final {
public:
    DockNode(DockNodeSharedPtr parent, WidgetSharedPtr rootWidget);
    ~DockNode() = default;

    glm::vec2 getOrigin() const;
    glm::vec2 getSize() const;

    void setOrigin(glm::vec2 value);
    void setSize(glm::vec2 value);
    void setRect(glm::vec2 origin, glm::vec2 size);

private:
    void updateWidget();

    DockNodeWeakPtr _parent;
    // RefList<DockNode> _children;
    WidgetSharedPtr _rootWidget;

    // Lower left corner (window Center).
    glm::vec2 _origin{};
    glm::vec2 _size{100.0f, 100.0f};
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(DockNode)

} // namespace rendell_ui