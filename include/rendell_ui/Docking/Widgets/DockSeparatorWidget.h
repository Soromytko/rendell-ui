#pragma once
#include <functional>
#include <memory>
#include <rendell_ui/Signal.h>
#include <rendell_ui/Widgets/RectangleWidget.h>
#include <rendell_ui/defines.h>

namespace rendell_ui {
class DockSeparatorWidget final : public RectangleWidget {
public:
    using NormalizeRatioCallback = std::function<float(float ratio)>;

    DockSeparatorWidget();
    ~DockSeparatorWidget() = default;

    // Widget
    void onMouseDown(glm::dvec2 cursorPosition) override;
    void onMouseHovered(glm::dvec2 cursorPosition) override;
    void onMouseExited() override;
    void onDragged(glm::dvec2 startPoint, glm::dvec2 endPoint) override;

    bool getIsHorizontal() const;
    NormalizeRatioCallback getNormalizeRatioCallback() const;
    float getThickness() const;
    float getRatio() const;

    void setThickness(float value);
    void setIsHorizontal(bool value);
    void setNormalizeRatioCallback(NormalizeRatioCallback callback);
    void setDockNodeRect(glm::vec2 size, glm::vec2 center);
    void setRatio(float value);

    Signal<void, float> ratioChanged;

private:
    // Widget
    glm::vec2 getRootPosition() const override;
    glm::vec2 getRootSize() const override;
    

    float getRatioWithDelta(float delta) const;

    void setDragOffset(float value);
    void updateRatioWithNewSize();
    void update();

    bool _isHorizontal{true};
    float _thickness{5.0f};
    NormalizeRatioCallback _normalizeRatioCallback{};
    glm::vec2 _dockNodeSize{};
    glm::vec2 _oldDockNodeSize{};
    glm::vec2 _dockNodeCenter{};
    float _startDragOffset{};
    float _dragOffset{};
};

RENDELL_UI_DECLARE_WIDGET(DockSeparatorWidget)

} // namespace rendell_ui