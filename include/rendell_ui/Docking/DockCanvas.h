#pragma once
#include "DockNode.h"
#include <rendell_ui/Canvas.h>
#include <rendell_ui/defines.h>

namespace rendell_ui {
class DockCanvas : public Canvas {
public:
    DockCanvas(ViewportSharedPtr viewport);
    virtual ~DockCanvas() = default;

    DockNodeSharedPtr getRootDockNode() const;

    void setRootDockNode(DockNodeSharedPtr rootDockNode);

protected:
    // Canvas
    virtual void onResized(glm::ivec2 size) override;

    DockNodeSharedPtr _rootDockNode{nullptr};

private:
    void updateRootDockNode(glm::vec2 size);
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(DockCanvas)

} // namespace rendell_ui