#pragma once
#include "../Signal.h"
#include "Widget.h"
#include <vector>

namespace rendell_ui {
class PageViewerWidget final : public Widget {
public:
    PageViewerWidget() = default;
    ~PageViewerWidget() = default;

    const std::vector<WidgetSharedPtr> &getPages() const;
    size_t getCurrentIndex() const;

    void setPages(std::vector<WidgetSharedPtr> pages);
    void addPage(WidgetSharedPtr page);
    bool removePage(WidgetSharedPtr page);
    bool removePage(size_t pageIndex);
    bool setCurrentIndex(int index);

    Signal<void, size_t> currentIndexChanged;

private:
    void updatePageVisibility();

    std::vector<WidgetSharedPtr> _pages;
    int _currentIndex{-1};
};

RENDELL_UI_DECLARE_WIDGET(PageViewerWidget)
} // namespace rendell_ui
