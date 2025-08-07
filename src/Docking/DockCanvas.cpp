#include <rendell_ui/Docking/DockCanvas.h>
#include <logging.h>

namespace rendell_ui {
DockCanvas::DockCanvas(ViewportSharedPtr viewport)
    : Canvas(viewport) {
}

DockNodeSharedPtr DockCanvas::getRootDockNode() const {
    return _rootDockNode;
}

void DockCanvas::setRootDockNode(DockNodeSharedPtr rootDockNode) {
    if (_rootDockNode != rootDockNode) {
        _rootDockNode = rootDockNode;
        updateRootDockNode(_viewport->getWindowSize());
    }
}

void DockCanvas::onResized(glm::ivec2 size) {
    Canvas::onResized(size);
    updateRootDockNode(_viewport->getWindowSize());
}

void DockCanvas::updateRootDockNode(glm::vec2 size) {
    if (_rootDockNode) {
        _rootDockNode->setRect(-size * 0.5f, size);
    }
}

} // namespace rendell_ui