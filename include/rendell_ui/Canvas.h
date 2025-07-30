#pragma once
#include "Viewport.h"
#include "Widgets/Widget.h"
#include "Window/WindowEventHandler.h"
#include <memory>
#include <set>
#include <unordered_set>

namespace rendell_ui {
class Canvas : public WindowEventHandler {
public:
    Canvas(ViewportSharedPtr viewport);
    virtual ~Canvas() = default;

    WidgetSharedPtr getRootWidget() const;

    void focusWidget(const WidgetSharedPtr &widget);

    ViewportSharedPtr getViewport() const;

    virtual void onRefreshed(int width, int height) override;
    virtual void onResized(int width, int height) override;
    virtual void onKeyInputted(const KeyboardInput &keyboardInput) override;
    virtual void onMouseButtonInputted(const MouseInput &mouseInput) override;
    virtual void onMouseMoved(double x, double y) override;
    virtual void onMouseScrolled(double x, double y) override;
    virtual void onCursorEntered(bool entered) override;
    virtual void onCharInputted(unsigned char character) override;

protected:
    glm::dvec2 convertCursorPositionToViewport(glm::dvec2 cursorPosition) const;
    bool containsMouseHoverWidgets(WidgetSharedPtr widget) const;

    virtual void onResized(glm::ivec2 size);

    void setFocusedWidget(const WidgetSharedPtr &widget);
    bool setCapturedWidget(const WidgetSharedPtr &widget, glm::dvec2 cursorPosition = {});
    WidgetSharedPtr hoverMouseRecursively(const WidgetSharedPtr &widget, glm::dvec2 cursor);

    WidgetSharedPtr _rootWidget;

    WidgetWeakPtr _focusedWidget;
    WidgetWeakPtr _capturedWidget;
    WidgetWeakPtr _hoveredWidget;

    glm::dvec2 _dragStartPoint{};

    ViewportSharedPtr _viewport;
    std::vector<WidgetWeakPtr> _mouseHoverWidgets;
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(Canvas)
} // namespace rendell_ui