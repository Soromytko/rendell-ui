#pragma once
#include "../src/Widgets/Transform2D.h"
#include "../src/Widgets/WidgetRegistrator.h"
#include <memory>
#include <rendell/oop/rendell_oop.h>
#include <rendell_ui/Signal.h>
#include <rendell_ui/Widgets/Anchor.h>
#include <rendell_ui/Widgets/AnchorableWidget.h>
#include <rendell_ui/Window/IWindow.h>
#include <rendell_ui/Window/window_input.h>
#include <rendell_ui/defines.h>
#include <unordered_set>

namespace rendell_ui {
class Widget;

RENDELL_UI_DECLARE_SHARED_PTR(Widget)
RENDELL_UI_DECLARE_WEAK_PTR(Widget)

void widget_deleter(Widget *widgetPtr);
void release_widget(const WidgetSharedPtr &widget);
void initWidget(WidgetSharedPtr widget, WidgetWeakPtr parent, bool registered = true);

template <typename WidgetType, typename... Args>
std::shared_ptr<WidgetType> createWidget(WidgetWeakPtr parent = {}, bool registered = true,
                                         Args &&...args) {
    std::shared_ptr<WidgetType> result(new WidgetType(std::forward<Args>(args)...), widget_deleter);
    initWidget(result, parent, registered);
    return result;
}

class Widget : public AnchorableWidget, public IWidget {
    friend void initWidget(WidgetSharedPtr widget, WidgetWeakPtr parent, bool registered);

public:
    Widget();
    virtual ~Widget();

protected:
    // AnchorableWidget
    void onMarkupUpdated() override;
    glm::vec2 getRootPosition() const override;
    glm::vec2 getRootSize() const override;

    virtual WindowCursorType getWindowCursorType() const;
    virtual void setWindowCursorType(WindowCursorType type);

private:
    void setSelfWeakPtr(WidgetWeakPtr value);
    void addChild(WidgetSharedPtr child);
    void removeChild(WidgetSharedPtr child);

public:
    void setVisible(bool value);
    bool getVisible() const override;

    void setInteract(bool value);
    bool getInteract() const override;

    bool getImplicitVisible() const override;

    void setParent(WidgetWeakPtr widget);
    WidgetWeakPtr getParent() const;
    const std::unordered_set<WidgetSharedPtr> &getChildren() const;

    const Transform2D &getTransform() const;

    void removeParent();

    void setName(const std::string &name);
    const std::string &getName() const override;

    virtual void setColor(glm::vec4 value);
    glm::vec4 getColor() const;

    // IWidget
    void updateRecursively() override;
    virtual void draw() override {};

    virtual void onFocused();
    virtual void onUnfocused();
    virtual void onMouseClick() {};
    virtual void onMouseDown(glm::dvec2 cursorPosition) {};
    virtual void onMouseUp(glm::dvec2 cursorPosition) {};

    virtual void onMouseHovered(glm::dvec2 cursorPosition) {}

    virtual void onMouseEntered() {}

    virtual void onMouseExited() {}

    virtual void onMouseScrolled(glm::dvec2 scroll) {}

    virtual void onCaptured(glm::dvec2 cursorPosition) {}

    virtual void onFreed(glm::dvec2 cursorPosition) {}

    virtual void onDragged(glm::dvec2 startPoint, glm::dvec2 endPoint) {}

    virtual void onKeyInputted(const KeyboardInput &keyboardInput) {};
    virtual void onCharInputted(unsigned char character) {};

    Signal<void> destroyed;
    Signal<void> focused;
    Signal<void> unfocused;
    Signal<void, bool> visibleChanged;
    Signal<void, bool> interactChanged;
    Signal<void, const WidgetSharedPtr &> parentChanged;
    Signal<void> childrenChanged;
    Signal<void, glm::vec4> colorChanged;
    Signal<void, const std::string &> nameChanged;

protected:
    virtual void updateUniforms();

    virtual void onSelfWeakPtrChanged() {}

    virtual void onParentChanged() {}

    virtual void onChildrenChanged() {}

    virtual void onVisibleChanged() {}

    virtual void onImplicitVisibleChanged() {}

    virtual void onInteractChanged() {}

    virtual void onColorChanged() {}

    virtual void onNameChanged() {}

    void updateImplicitVisibleRecursively();

    WidgetWeakPtr _selfWeakPtr;

    bool _visible{true};
    bool _implicitVisible{true};
    bool _interact{true};

    WidgetWeakPtr _parent{};
    std::unordered_set<WidgetSharedPtr> _children{};

    std::string _name{"Widget"};

    glm::vec4 _color{1.0f, 1.0f, 1.0f, 1.0f};
    rendell::oop::ShaderProgramSharedPtr _shaderProgram{nullptr};
    rendell::oop::Mat4Uniform _matrixUniform{"u_Matrix"};
    rendell::oop::Float2Uniform _sizeUniform{"u_Size"};
    rendell::oop::Float4Uniform _colorUniform{"u_Color"};
};

RENDELL_UI_DECLARE_WIDGET(Widget)

} // namespace rendell_ui
