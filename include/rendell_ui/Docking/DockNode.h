#pragma once
#include "Widgets/DockRootWidget.h"
#include "Widgets/DockSeparatorWidget.h"
#include <memory>
#include <rendell_ui/Widgets/Widget.h>
#include <rendell_ui/defines.h>

namespace rendell_ui {
class DockNode;

RENDELL_UI_DECLARE_SHARED_PTR(DockNode)
RENDELL_UI_DECLARE_WEAK_PTR(DockNode)

class DockNode final : public std::enable_shared_from_this<DockNode> {
public:
    enum class SplitType {
        left,
        right,
        bottom,
        top,
    };

    using CreateDockSeparatorCallback =
        std::function<DockSeparatorWidgetSharedPtr(WidgetSharedPtr parent)>;

    static void setCreateDockSeparatorCallback(CreateDockSeparatorCallback callback);

    DockNode(DockNodeSharedPtr parent, WidgetSharedPtr canvasRootWidget);
    ~DockNode() = default;

    const std::string &getHeader() const;
    glm::vec2 getOrigin() const;
    glm::vec2 getSize() const;
    glm::vec2 getMinSize() const;
    DockRootWidgetSharedPtr getRootWidget() const;
    WidgetSharedPtr getWorkSpaceWidget() const;
    DockNodeSharedPtr getParent() const;
    DockNodeSharedPtr getFirstChild() const;
    DockNodeSharedPtr getSecondChild() const;
    DockSeparatorWidgetSharedPtr getSeparator() const;
    bool hasChildren() const;

    void setHeader(const std::string &header);
    void setOrigin(glm::vec2 value);
    void setSize(glm::vec2 value);
    void setMinSize(glm::vec2 value);
    void setRect(glm::vec2 origin, glm::vec2 size);

    bool splitBy(DockNodeSharedPtr dockNode, SplitType type);
    bool release();

private:
    glm::vec2 getCenter() const;
    glm::vec2 getDeltaSize(glm::vec2 newSize) const;
    DockRootWidgetSharedPtr createRootWidget() const;
    float getNormalizedRatio(float ratio) const;

    void setRootWidget(DockRootWidgetSharedPtr widget);
    void replaceWith(DockNodeSharedPtr dockNode);
    void onRatioChanged(float ratio);
    void updateRootWidget();
    void updateChildren(float ratio);
    void update();

    std::string _header{};

    DockNodeWeakPtr _parent;
    DockNodeSharedPtr _firstChild{nullptr};
    DockNodeSharedPtr _secondChild{nullptr};
    DockRootWidgetSharedPtr _rootWidget;
    WidgetSharedPtr _canvasRootWidget;

    DockSeparatorWidgetSharedPtr _dockSeparatorWidget{nullptr};

    // Lower left corner (window Center).
    glm::vec2 _origin{};
    glm::vec2 _size{100.0f, 100.0f};
    glm::vec2 _minSize{100.0f, 100.0f};
    int _depth{0};

    bool _isChildrenUpdated{false};
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(DockNode)

} // namespace rendell_ui