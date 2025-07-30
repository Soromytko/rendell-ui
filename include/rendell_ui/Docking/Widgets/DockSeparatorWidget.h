#pragma once
#include <functional>
#include <memory>
#include <rendell_ui/Docking/DockNode.h>
#include <rendell_ui/Docking/IDockNodeSeparator.h>
#include <rendell_ui/Signal.h>
#include <rendell_ui/Widgets/RectangleWidget.h>
#include <rendell_ui/defines.h>

namespace rendell_ui {
class DockSeparatorWidget final : public RectangleWidget, public IDockNodeSeparator {
public:
    DockSeparatorWidget();
    ~DockSeparatorWidget() = default;

    // Widget
    void onMouseDown(glm::dvec2 cursorPosition) override;
    void onMouseHovered(glm::dvec2 cursorPosition) override;
    void onMouseExited() override;
    void onDragged(glm::dvec2 startPoint, glm::dvec2 endPoint) override;

    // IDockNodeSeparator
    bool getIsHorizontal() const override;
    float getRatio() const override;
    float getThickness() const override;

    void setIsHorizontal(bool value) override;
    void setRatio(float value) override;
    void setThickness(float value) override;
    void setDockNodeRect(glm::vec2 dockNodeCenter, glm::vec2 dockNodeSize) override;
    void setOnDraggedCallback(OnDraggedCallback callback) override;

    void release() override;

private:
    // Widget
    glm::vec2 getRootPosition() const override;
    glm::vec2 getRootSize() const override;

    float getRatioWithDelta(float delta) const;

    void update();

    bool _isHorizontal{true};
    float _thickness{5.0f};
    float _startRatio{};
    float _ratio{0.5f};
    glm::vec2 _dockNodeCenter{};
    glm::vec2 _dockNodeSize{};
    OnDraggedCallback _onDraggedCallback{};
};

RENDELL_UI_DECLARE_WIDGET(DockSeparatorWidget)

} // namespace rendell_ui