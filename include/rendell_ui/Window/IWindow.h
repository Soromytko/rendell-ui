#pragma once
#include <glm/glm.hpp>
#include <rendell_ui/defines.h>

namespace rendell_ui {
enum class WindowCursorType {
    arrow,
    hand,
    cross,
    ibeam,
    verticalResize,
    horizontalResize,
    nvseResize,
    neswResize,
    centerResize,
};

class IWindow {
public:
    IWindow() = default;
    virtual ~IWindow() = default;

    virtual glm::ivec2 getSize() const = 0;
    virtual glm::ivec2 getMinSize() const = 0;
    virtual WindowCursorType getCursorType() const = 0;

    virtual void setSize(glm::ivec2 value) = 0;
    virtual void setMinSize(glm::ivec2 value) = 0;
    virtual void setCursorType(WindowCursorType type) = 0;
};

RENDELL_UI_DECLARE_WEAK_PTR(IWindow)

} // namespace rendell_ui