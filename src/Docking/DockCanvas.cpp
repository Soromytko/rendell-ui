#include <rendell_ui/Docking/DockCanvas.h>

namespace rendell_ui {
DockCanvas::DockCanvas(ViewportSharedPtr viewport)
    : Canvas(viewport) {
    _rootDockNode = makeDockNode(nullptr, _rootWidget);
}

DockNodeSharedPtr DockCanvas::getRootDockNode() const {
    return _rootDockNode;
}

void DockCanvas::onRefreshed(int width, int height) {
    _viewport->setWindowSize({width, height});
    _viewport->setSize(width, height);
    _viewport->setOffset(0, 0);
    updateDockNodeRect(width, height);
}

void DockCanvas::onResized(int width, int height) {
    _viewport->setWindowSize({width, height});
    _viewport->setSize(width, height);
    _viewport->setOffset(0, 0);
    updateDockNodeRect(width, height);
}

void DockCanvas::updateDockNodeRect(int width, int height) {
    const glm::vec2 size{width, height};
    _rootDockNode->setRect(-size * 0.5f, size);
}

} // namespace rendell_ui