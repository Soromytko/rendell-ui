#include "WindowManager.h"

#include <logging.h>

#include <algorithm>
#include <cassert>

namespace rendell_ui {
static WindowManager *s_instance{nullptr};

void WindowManager::init() {
    assert(!s_instance);
    s_instance = new WindowManager;
}

void WindowManager::release() {
    assert(s_instance);
    delete s_instance;
}

WindowManager *WindowManager::getInstance() {
    assert(s_instance);
    return s_instance;
}

bool WindowManager::isValid(IWindow *window) const {
    return _windows.find(window) != _windows.end();
}

size_t WindowManager::getWindowCount() const {
    return _windows.size();
}

std::vector<IWindow *> WindowManager::getWindows() const {
    std::vector<IWindow *> result(_windows.size());
    std::transform(_windows.begin(), _windows.end(), result.begin(),
                   [](const auto &key) { return key.second.get(); });
    return result;
}

void WindowManager::reserveWindow() {
    _reservedWindow = makeWindow(200, 200, "", true);
    if (!_reservedWindow->isInitialized()) {
        RUI_CRITICAL("Window initialization error");
        _reservedWindow.reset();
    }
}

IWindow *WindowManager::createWindow(uint32_t width, uint32_t height, const char *title,
                                     bool isHidden) {
    if (_reservedWindow) {
        WindowSharedPtr window = std::move(_reservedWindow);
        window->setSize({width, height});
        window->setTitle(title);
        if (!isHidden) {
            window->show();
        }
        _windows[window.get()] = window;
        return window.get();
    }

    WindowSharedPtr window = makeWindow(width, height, title, isHidden);
    _windows[window.get()] = window;
    return window.get();
}

void WindowManager::releaseWindow(IWindow *window) {
    assert(window);
    auto it = _windows.find(window);
    if (it == _windows.end()) {
        return;
    }
    _windows.erase(it);
}

} // namespace rendell_ui