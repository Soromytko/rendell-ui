#pragma once
#include "../StringListModel.h"
#include "TabButtonWidget.h"
#include "Widget.h"
#include <initializer_list>

namespace rendell_ui {
class TabBarWidget final : public Widget {
public:
    using CreateTabButtonDelegate = std::function<TabButtonWidgetSharedPtr(std::wstring)>;

    TabBarWidget(std::initializer_list<StringListModel::String> tabNames = {});
    ~TabBarWidget() = default;

    StringListModelSharedPtr getModel() const;
    CreateTabButtonDelegate getCreateTabButtonDelegate() const;
    void draw() override;

    void setModel(StringListModelSharedPtr model);
    void setCreateTabButtonDelegate(CreateTabButtonDelegate delegate);
    void resetCreateTabButtonDelegate();
    bool setCurrentIndex(int index);

    Signal<void, int> currentIndexChanged;

private:
    void onSelfWeakPtrChanged() override;
    void onMarkupUpdated() override;

    void updateButtonSelection();
    void selectButton(TabButtonWidget *button);

    void onModelItemAdded(const std::wstring &item, size_t itemIndex);
    void onModelItemRemoved(const std::wstring &item, size_t itemIndex);
    void onModelItemChanged(const std::wstring &item, size_t itemIndex);
    void onTabButtonIsSelectedChanged(TabButtonWidget *button);

    int getButtonIndexUnderCursor(glm::dvec2 cursorPosition) const;

    TabButtonWidgetSharedPtr createTabButton(std::wstring title);

    void updateButtons();
    void updateButtonMarkup();

    signal_connection_id_t _itemAddedConnectionId;
    signal_connection_id_t _itemRemovedConnectionId;
    signal_connection_id_t _itemChangedConnectionId;

    int _currentIndex{-1};
    CreateTabButtonDelegate _createTabButtonDelegate;
    StringListModelSharedPtr _model;
    std::vector<TabButtonWidgetSharedPtr> _buttons;
};

RENDELL_UI_DECLARE_WIDGET(TabBarWidget)
} // namespace rendell_ui
