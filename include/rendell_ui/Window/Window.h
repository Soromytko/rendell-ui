#pragma once
#include "IWindow.h"
#include "WindowEventHandler.h"
#include "window_input.h"
#include <glm/glm.hpp>

struct GLFWwindow;
struct GLFWcursor;

namespace rendell_ui {
class Window : public IWindow {
public:
    Window(int width, int height, const char *title);
    virtual ~Window();

    size_t getRendellContextId() const;

    // IWindow
    WindowCursorType getCursorType() const override;
    void setCursorType(WindowCursorType type) override;

    bool isOpen();
    void processEvents();
    static bool isInitialized();
    static int getWindowCount();

    void setEventHandler(WindowEventHandlerSharedPtr eventHandler);

    void waitEvent() const;
    void *getNativeWindowHandle() const;
    void *getX11Display() const;
    glm::ivec2 getSize() const;
    glm::dvec2 getCursorPosition() const;
    WindowEventHandlerSharedPtr getEventHandler() const;

private:
    bool init();
    void setupWindowCallbacks();

protected:
    GLFWwindow *_glfwWindow;
    GLFWcursor *_glfwCursor{nullptr};
    WindowEventHandlerSharedPtr _eventHandler{nullptr};
    static bool _glfwInitialized;
    static int _windowCount;
    WindowCursorType _cursorType{WindowCursorType::arrow};
    size_t _rendell_context_id;
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(Window)
} // namespace rendell_ui