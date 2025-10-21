#include "Window.h"

#include "window_callbacks.h"
#include <GLFW/glfw3.h>
#include <logging.h>
#include <rendell/init.h>
#include <rendell/rendell.h>

#if defined(_WIN32)
#define NOMINMAX
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#endif

// Static stuff
namespace rendell_ui {
static bool s_glfwInitialized{false};
static WindowEventHandlerSharedPtr s_eventHandlerStub;

static rendell::NativeViewId register_native_view(Window *window) {
    rendell::NativeView nativeView;
    nativeView.nativeWindowHandle = window->getNativeWindowHandle();
#if defined(__linux__)
    nativeView.x11Display = window->getX11Display();
#endif
    return rendell::registerNativeView(nativeView);

    /*  if (rendell::context_id context = rendell::init(initer)) {
          rendell::setPixelUnpackAlignment(1);
          return context;
      }*/
    return 0;
}

static bool init_glfw() {
    assert(!s_glfwInitialized);
#if defined(__linux__)
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
    return glfwInit() == GLFW_TRUE;
}

static void release_glfw_cursor(GLFWcursor *glfwCursor) {
    if (glfwCursor) {
        glfwDestroyCursor(glfwCursor);
    }
}

} // namespace rendell_ui

// Window implementation
namespace rendell_ui {
Window::Window(uint32_t width, uint32_t height, const char *title, bool isHidden) {
    assert(width > 0);
    assert(height > 0);
    if (!s_glfwInitialized) {
        s_glfwInitialized = init_glfw();
    }

    if (!s_glfwInitialized) {
        RUI_ERROR("Failure to initialize the GLFW");
        return;
    }

    if (isHidden) {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    _glfwWindow =
        glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title, NULL, NULL);
    if (!_glfwWindow) {
        RUI_ERROR("Failure to create GLFW window");
        return;
    }
    _windowCount++;

    setupWindowCallbacks();

    _nativeViewId = register_native_view(this);
    assert(_nativeViewId != rendell::InvalidNativeViewId);
}

bool Window::isInitialized() {
    return s_glfwInitialized && _glfwWindow != nullptr;
}

Window::~Window() {
    rendell::unregisterNativeView(_nativeViewId);

    if (_glfwCursor) {
        release_glfw_cursor(_glfwCursor);
    }

    if (_glfwWindow) {
        _windowCount--;
        if (_windowCount <= 0) {
            glfwTerminate();
            s_glfwInitialized = false;
        }
    }
}

glm::ivec2 Window::getSize() const {
    assert(_glfwWindow);
    int width, height;
    glfwGetFramebufferSize(_glfwWindow, &width, &height);
    return glm::ivec2(width, height);
}

glm::ivec2 Window::getMinSize() const {
    return _minSize;
}

glm::dvec2 Window::getCursorPosition() const {
    assert(_glfwWindow);
    double x, y;
    glfwGetCursorPos(_glfwWindow, &x, &y);
    return (static_cast<glm::dvec2>(getSize()) * 0.5 - glm::dvec2(x, y)) * glm::dvec2(-1.0, 1.0);
}

WindowEventHandlerSharedPtr Window::getEventHandler() const {
    return _eventHandler;
}

rendell::NativeViewId Window::getNativeViewId() const {
    return _nativeViewId;
}

WindowCursorType Window::getCursorType() const {
    return _cursorType;
}

static int get_glfw_cursor_type(WindowCursorType type) {
    switch (type) {
    case WindowCursorType::arrow:
        return GLFW_ARROW_CURSOR;
    case WindowCursorType::hand:
        return GLFW_HAND_CURSOR;
    case WindowCursorType::cross:
        return GLFW_CROSSHAIR_CURSOR;
    case WindowCursorType::ibeam:
        return GLFW_IBEAM_CURSOR;
    case WindowCursorType::verticalResize:
        return GLFW_VRESIZE_CURSOR;
    case WindowCursorType::horizontalResize:
        return GLFW_HRESIZE_CURSOR;
    case WindowCursorType::nvseResize:
        return GLFW_RESIZE_NWSE_CURSOR;
    case WindowCursorType::neswResize:
        return GLFW_RESIZE_NESW_CURSOR;
    case WindowCursorType::centerResize:
        return GLFW_RESIZE_ALL_CURSOR;
    }

    return GLFW_ARROW_CURSOR;
}

void Window::setTitle(const char *title) {
    assert(_glfwWindow);
    glfwSetWindowTitle(_glfwWindow, title);
}

void Window::setSize(glm::ivec2 value) {
    assert(_glfwWindow);
    glfwSetWindowSize(_glfwWindow, value.x, value.y);
}

void Window::setMinSize(glm::ivec2 value) {
    assert(_glfwWindow);
    if (_minSize != value) {
        _minSize = value;
        glfwSetWindowSizeLimits(_glfwWindow, _minSize.x, _minSize.y, GLFW_DONT_CARE,
                                GLFW_DONT_CARE);

        const glm::ivec2 newSize = glm::max(_minSize, getSize());
        setSize(newSize);
    }
}

void Window::setCursorType(WindowCursorType type) {
    assert(_glfwWindow);
    if (_cursorType != type) {
        _cursorType = type;

        release_glfw_cursor(_glfwCursor);
        _glfwCursor = glfwCreateStandardCursor(get_glfw_cursor_type(type));
        glfwSetCursor(_glfwWindow, _glfwCursor);
    }
}

void Window::setEventHandler(WindowEventHandlerSharedPtr eventHandler) {
    _eventHandler = eventHandler;
    _eventHandler->setWindow(this);
}

void Window::show() {
    assert(_glfwWindow);
    glfwShowWindow(_glfwWindow);
}

void Window::hide() {
    assert(_glfwWindow);
    glfwHideWindow(_glfwWindow);
}

bool Window::isOpen() const {
    assert(_glfwWindow);
    return !glfwWindowShouldClose(_glfwWindow);
}

void Window::waitEvent() const {
    glfwWaitEvents();
}

void Window::processEvents() const {
    glfwPollEvents();
}

void *Window::getNativeWindowHandle() const {
    assert(_glfwWindow);
#if defined(_WIN32)
    return (void *)glfwGetWin32Window(_glfwWindow);
#elif defined(__linux__)
    return (void *)(uintptr_t)glfwGetX11Window(_glfwWindow);
#endif
    return nullptr;
}

void *Window::getX11Display() const {
#if defined(__linux__)
    return (void *)glfwGetX11Display();
#endif
    return nullptr;
}

void Window::setupWindowCallbacks() {
    assert(_glfwWindow);
    if (!_eventHandler) {
        if (!s_eventHandlerStub) {
            s_eventHandlerStub = makeWindowEventHandler();
        }
        _eventHandler = s_eventHandlerStub;
    }
    glfwSetWindowUserPointer(_glfwWindow, static_cast<void *>(this));

    glfwSetFramebufferSizeCallback(_glfwWindow, resizeWindowCallback);
    glfwSetWindowRefreshCallback(_glfwWindow, refreshWindowCallback);
    glfwSetKeyCallback(_glfwWindow, inputKeyCallback);
    glfwSetMouseButtonCallback(_glfwWindow, inputMouseBottonCallback);
    glfwSetCursorPosCallback(_glfwWindow, moveMouseCallback);
    glfwSetCursorEnterCallback(_glfwWindow, enterCursorCallback);
    glfwSetScrollCallback(_glfwWindow, scrollMouseCallback);
    glfwSetCharCallback(_glfwWindow, inputCharCallback);
}

int Window::_windowCount = 0;
} // namespace rendell_ui
