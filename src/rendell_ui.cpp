#include <rendell_ui/rendell_ui.h>

#include "Window/WindowManager.h"

namespace rendell_ui {
IWindow *createWindow(uint32_t width, uint32_t height, const char *title, bool isHidden) {
    return WindowManager::getInstance()->createWindow(width, height, title, isHidden);
}

void releaseWindow(IWindow *window) {
    WindowManager::getInstance()->releaseWindow(window);
}

size_t getWindowCount() {
    return WindowManager::getInstance()->getWindowCount();
}

std::vector<IWindow *> getWindows() {
    return WindowManager::getInstance()->getWindows();
}

bool isValidWindow(IWindow *window) {
    return WindowManager::getInstance()->isValid(window);
}
} // namespace rendell_ui