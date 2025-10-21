#pragma once
#include "RectangleWidget.h"
#include "TextWidget.h"
#include "Widget.h"
#include <string>

namespace rendell_ui {
class TabButtonWidget : public Widget {
public:
    TabButtonWidget(std::wstring title = {});
    virtual ~TabButtonWidget() = default;

    bool getIsSelected() const;
    const std::wstring &getTitle() const;

    void draw() override;

    void setTitle(std::wstring value);
    void setIsSelected(bool value);

    Signal<void, TabButtonWidget *> isSelectedChanged;

protected:
    virtual void onMouseEntered() override;
    virtual void onMouseExited() override;
    virtual void onIsSelectedChanged();

    bool _isSelected{false};

private:
    void onSelfWeakPtrChanged() override;
    void onColorChanged() override;
    void onMouseDown(glm::dvec2 cursorPosition) override;

    std::wstring _title;
    RectangleWidgetSharedPtr _rectangle;
    TextWidgetSharedPtr _textWidget;
};

RENDELL_UI_DECLARE_WIDGET(TabButtonWidget)
} // namespace rendell_ui
