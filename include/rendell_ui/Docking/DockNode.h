#pragma once
#include "DockSeparator.h"
#include <memory>
#include <rendell_ui/Widgets/Widget.h>
#include <rendell_ui/defines.h>

namespace rendell_ui {
class DockNode;

RENDELL_UI_DECLARE_SHARED_PTR(DockNode)
RENDELL_UI_DECLARE_WEAK_PTR(DockNode)

//void splitDockNode(DockNodeSharedPtr node, WidgetSharedPtr rootWidget);

class DockNode final : public std::enable_shared_from_this<DockNode> {
public:
    enum class SplitType {
        left,
        right,
        bottom,
        top,
    };

    DockNode(DockNodeSharedPtr parent, WidgetSharedPtr rootWidget);
    ~DockNode() = default;

    glm::vec2 getOrigin() const;
    glm::vec2 getSize() const;
    WidgetSharedPtr getRootWidget() const;
    DockNodeSharedPtr getParent() const;
    DockNodeSharedPtr getFirstChild() const;
    DockNodeSharedPtr getSecondChild() const;
    bool hasChildren() const;

    void setOrigin(glm::vec2 value);
    void setSize(glm::vec2 value);
    void setRect(glm::vec2 origin, glm::vec2 size);
    void setChildren(DockNodeSharedPtr firstChild, DockNodeSharedPtr secondChild);
    void setFirstChild(DockNodeSharedPtr firstChild);
    void setSecondChild(DockNodeSharedPtr secondChild);

    bool splitBy(DockNodeSharedPtr dockNode, SplitType type);
    bool release();

private:
    void replaceWith(DockNodeSharedPtr dockNode);
    void updateWidget();
    void update();

    DockNodeWeakPtr _parent;
    DockNodeSharedPtr _firstChild{nullptr};
    DockNodeSharedPtr _secondChild{nullptr};
    WidgetSharedPtr _rootWidget;

    // Lower left corner (window Center).
    glm::vec2 _origin{};
    glm::vec2 _size{100.0f, 100.0f};
    float _ratio = 1.0f;
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(DockNode)

} // namespace rendell_ui