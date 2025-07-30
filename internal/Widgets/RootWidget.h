#pragma once
#include <memory>
#include <rendell_ui/Widgets/Widget.h>
#include <rendell_ui/Window/WindowEventHandler.h>
#include <rendell_ui/defines.h>

namespace rendell_ui {
class RootWidget : public Widget {
public:
    RootWidget();
    virtual ~RootWidget() = default;

    WindowEventHandler *getWindowEventHandler() const;
    void setWindowEventHandler(WindowEventHandler *windowEventHandler);

private:
    // Widget
    WindowCursorType getWindowCursorType() const override;
    void setWindowCursorType(WindowCursorType type) override;

    WindowEventHandler *_windowEventHandler{nullptr};
};

RENDELL_UI_DECLARE_WIDGET(RootWidget)
} // namespace rendell_ui