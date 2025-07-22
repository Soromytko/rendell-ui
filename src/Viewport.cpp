#include <rendell/rendell.h>
#include <rendell_ui/Viewport.h>

namespace rendell_ui {
static ViewportSharedPtr s_currentViewport{nullptr};

void Viewport::setCurrent(const ViewportSharedPtr viewport) {
    s_currentViewport = viewport;
}

ViewportSharedPtr Viewport::getCurrent() {
    return s_currentViewport;
}

void Viewport::apply() {
    rendell::setViewport(_x, _y, _width, _height, _windowSize.x, _windowSize.y);
}

void Viewport::setParameters(int x, int y, int width, int height) {
    _x = x;
    _y = y;
    _width = width;
    _height = height;
}

void Viewport::setWindowSize(glm::ivec2 value) {
    _windowSize = value;
}

void Viewport::setSize(int width, int height) {
    _width = width;
    _height = height;
}

void Viewport::startScissors(int x, int y, int width, int height) {
    rendell::startScissors(x, y, width, height);
}

void Viewport::endScissors() {
    rendell::endScissors();
}

void Viewport::setProjectMat(const glm::mat4 &value) {
    _projectMat = value;
}

void Viewport::setViewMat(const glm::mat4 &value) {
    _viewMat = value;
}

glm::ivec2 Viewport::getWindowSize() const {
    return _windowSize;
}

const glm::mat4 &Viewport::getProjectMat() const {
    return _projectMat;
}

const glm::mat4 &Viewport::getViewMat() const {
    return _viewMat;
}

glm::ivec2 Viewport::getSize() const {
    return glm::ivec2({_width, _height});
}

glm::ivec2 Viewport::convertCursorPosition(glm::ivec2 cursorPosition) const {
    const glm::ivec2 localViewportOffset =
        glm::ivec2(-_windowSize.x, _windowSize.y) / 2 + glm::ivec2(_x, -_y);
    const glm::ivec2 viewportCenter = localViewportOffset + glm::ivec2(_width, -_height) / 2;
    return cursorPosition - viewportCenter;
}

glm::ivec2 Viewport::getOffset() const {
    return glm::ivec2({_x, _y});
}
} // namespace rendell_ui
