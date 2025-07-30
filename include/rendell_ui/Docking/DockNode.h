#pragma once
#include "IDockNodeSeparator.h"
#include "IDockable.h"
#include "Widgets/DockSeparatorWidget.h"
#include <functional>
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

    using RequestParentResizeCallback = std::function<void(glm::vec2)>;

    DockNode(DockNodeSharedPtr parent);
    ~DockNode() = default;

    float getRatio() const;
    bool getIsHorizontal() const;
    glm::vec2 getOrigin() const;
    glm::vec2 getSize() const;
    glm::vec2 getMinSize() const;
    DockNodeSharedPtr getParent() const;
    DockNodeSharedPtr getFirstChild() const;
    DockNodeSharedPtr getSecondChild() const;
    IDockNodeSeparatorSharedPtr getSeparator() const;
    RequestParentResizeCallback getRequestParentResizeCallback() const;
    const std::vector<IDockableSharedPtr> &getDockables() const;

    bool hasChildren() const;

    void setRatio(float value);
    void setIsHorizontal(bool value);
    void setOrigin(glm::vec2 value);
    void setSize(glm::vec2 value);
    void setRect(glm::vec2 origin, glm::vec2 size);
    void setChildren(DockNodeSharedPtr firstChild, DockNodeSharedPtr secondChild);
    void setSeparator(IDockNodeSeparatorSharedPtr separator);
    void setRequestParentResizeCallback(RequestParentResizeCallback callback);
    void setDockables(std::vector<IDockableSharedPtr> dockables);

    void moveDockablesTo(DockNodeSharedPtr dockNodeReceiver);
    void takeDockablesFrom(DockNodeSharedPtr dockNodeSender);

private:
    glm::vec2 getCenter() const;
    glm::vec2 getDeltaSize(glm::vec2 newSize) const;
    float normalizeRatio(float ratio) const;

    void setMinSize(glm::vec2 value);

    void updateMinSizeAccordingChildren();
    void updateMinSizeAccordingDockables();
    void updateChildren();
    void update();
    void updateDockables();
    void updateRatio();
    void updateSeparator();

    DockNodeWeakPtr _parent;
    DockNodeSharedPtr _firstChild{nullptr};
    DockNodeSharedPtr _secondChild{nullptr};
    std::vector<IDockableSharedPtr> _dockables{};

    int _depth{0};
    float _ratio{0.5f};
    bool _isHorizontal{false};

    // Lower left corner (window Center).
    glm::vec2 _origin{};
    glm::vec2 _size{100.0f, 100.0f};
    glm::vec2 _minSize{100.0f, 100.0f};

    bool _isChildrenUpdated{false};

    IDockNodeSeparatorSharedPtr _separator{nullptr};
    RequestParentResizeCallback _requestParentResizeCallback{nullptr};
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(DockNode)

} // namespace rendell_ui