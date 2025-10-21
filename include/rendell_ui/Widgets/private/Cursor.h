#pragma once
#include <rendell_ui/Timer.h>
#include <rendell_ui/Widgets/RectangleWidget.h>

namespace rendell_ui {
class Cursor final : public RectangleWidget {
public:
    Cursor();
    ~Cursor() = default;

    void draw() override;

    void setThickness(float value);
    void setHeight(float value);
    void setVerticalOffset(float value);
    void setBlinkTimeout(size_t ms);
    void resetBlinkTimer();

    size_t getBlinkTimeout() const;
    float getThickness() const;
    float getHeight() const;
    float getVerticalOffset() const;

private:
    void onVisibleChanged() override;

    float _verticalOffset{0.0f};
    bool _shouldDraw{false};
    TimerSharedPtr _timer;
};

RENDELL_UI_DECLARE_WIDGET(Cursor)

} // namespace rendell_ui
