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

    // Canvas
    virtual void onRefreshed(int width, int height) override;
    virtual void onResized(int width, int height) override;

private:
    void updateDockNodeRect(int width, int height);

    DockNodeSharedPtr _rootDockNode;
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(DockCanvas)

} // namespace rendell_ui