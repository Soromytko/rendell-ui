#pragma once
#include "Window.h"

#include <unordered_map>
#include <vector>

namespace rendell_ui {
class WindowManager final {
public:
    static void init();
    static void release();
    static WindowManager *getInstance();

private:
    WindowManager() = default;

public:
    ~WindowManager() = default;

    bool isValid(IWindow *window) const;
    size_t getWindowCount() const;
    std::vector<IWindow *> getWindows() const;

    void reserveWindow();
    IWindow *createWindow(uint32_t width, uint32_t height, const char *title, bool isHidden);
    void releaseWindow(IWindow *window);

private:
    WindowSharedPtr _reservedWindow{nullptr};
    std::unordered_map<IWindow *, WindowSharedPtr> _windows{};
};

} // namespace rendell_ui