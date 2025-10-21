#pragma once
#include <rendell/init.h>
#include <rendell_ui/Window/IWindow.h>

struct GLFWwindow;
struct GLFWcursor;

namespace rendell_ui {
class Window final : public IWindow {
public:
    Window(uint32_t width, uint32_t height, const char *title, bool isHidden = false);
    ~Window();

    bool isInitialized();

    glm::ivec2 getSize() const override;
    glm::ivec2 getMinSize() const override;
    glm::dvec2 getCursorPosition() const override;
    WindowCursorType getCursorType() const override;
    WindowEventHandlerSharedPtr getEventHandler() const override;
    rendell::NativeViewId getNativeViewId() const override;

    void setTitle(const char *title) override;
    void setSize(glm::ivec2 value) override;
    void setMinSize(glm::ivec2 value) override;
    void setCursorType(WindowCursorType type) override;
    void setEventHandler(WindowEventHandlerSharedPtr eventHandler) override;

    void show() override;
    void hide() override;

    bool isOpen() const override;
    void waitEvent() const override;
    void processEvents() const override;

    void *getNativeWindowHandle() const;
    void *getX11Display() const;

private:
    void setupWindowCallbacks();

protected:
    GLFWwindow *_glfwWindow{nullptr};
    GLFWcursor *_glfwCursor{nullptr};
    WindowEventHandlerSharedPtr _eventHandler{nullptr};
    static int _windowCount;
    WindowCursorType _cursorType{WindowCursorType::arrow};
    rendell::NativeViewId _nativeViewId{rendell::InvalidNativeViewId};

    glm::ivec2 _minSize{0, 0};
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(Window)
} // namespace rendell_ui