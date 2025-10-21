#pragma once
#include <rendell_ui/Window/WindowEventHandler.h>
#include <rendell_ui/Window/window_input.h>

#include <rendell/init.h>

#include <glm/glm.hpp>

namespace rendell_ui {
class IWindow {
protected:
    IWindow() = default;

public:
    virtual ~IWindow() = default;

    virtual glm::ivec2 getSize() const = 0;
    virtual glm::ivec2 getMinSize() const = 0;
    virtual glm::dvec2 getCursorPosition() const = 0;
    virtual WindowCursorType getCursorType() const = 0;
    virtual WindowEventHandlerSharedPtr getEventHandler() const = 0;
    virtual rendell::NativeViewId getNativeViewId() const = 0;

    virtual void setTitle(const char *title) = 0;
    virtual void setSize(glm::ivec2 value) = 0;
    virtual void setMinSize(glm::ivec2 value) = 0;
    virtual void setCursorType(WindowCursorType type) = 0;
    virtual void setEventHandler(WindowEventHandlerSharedPtr eventHandler) = 0;

    virtual void show() = 0;
    virtual void hide() = 0;

    virtual bool isOpen() const = 0;
    virtual void waitEvent() const = 0;
    virtual void processEvents() const = 0;
};
} // namespace rendell_ui