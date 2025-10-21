#include <rendell_ui/Window/WindowEventHandler.h>

#include <rendell_ui/Window/IWindow.h>

namespace rendell_ui {
WindowCursorType WindowEventHandler::getWindowCursorType() const {
    return _window->getCursorType();
}

void WindowEventHandler::setWindow(IWindow *window) {
    _window = window;
}

void WindowEventHandler::setWindowCursor(WindowCursorType type) {
    _window->setCursorType(type);
}

} // namespace rendell_ui
