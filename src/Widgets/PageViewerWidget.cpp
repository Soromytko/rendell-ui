#include <algorithm>
#include <logging.h>
#include <rendell_ui/Widgets/PageViewerWidget.h>

namespace rendell_ui {
const std::vector<WidgetSharedPtr> &PageViewerWidget::getPages() const {
    return _pages;
}

size_t PageViewerWidget::getCurrentIndex() const {
    return _currentIndex;
}

void PageViewerWidget::setPages(std::vector<WidgetSharedPtr> pages) {
    _pages = std::move(pages);
    const auto oldCurrentSignal = _currentIndex;
    _currentIndex = 0;
    updatePageVisibility();
    if (_currentIndex != oldCurrentSignal) {
        currentIndexChanged.emit(_currentIndex);
    }
}

void PageViewerWidget::addPage(WidgetSharedPtr page) {
    _pages.push_back(page);
    const int newIndex = std::clamp(_currentIndex, 0, static_cast<int>(_pages.size()) - 1);
    setCurrentIndex(newIndex);
}

bool PageViewerWidget::removePage(WidgetSharedPtr page) {
    auto it = std::find(_pages.begin(), _pages.end(), page);
    if (it == _pages.end()) {
        RUI_ERROR("The \"{}\" widget is not the Page Widget", page->getName());
        return false;
    }

    _pages.erase(it);
    return true;
}

bool PageViewerWidget::removePage(size_t pageIndex) {
    if (pageIndex >= _pages.size()) {
        RUI_ERROR("Invalid index; index = {}, _page.size() = {}", pageIndex, _pages.size());
        return false;
    }

    _pages.erase(_pages.begin() + pageIndex);

    if (_currentIndex >= pageIndex) {
        if (_pages.size() == 0) {
            setCurrentIndex(-1);
        } else {
            const int newIndex =
                std::clamp(_currentIndex + 1, 0, static_cast<int>(_pages.size()) - 1);
            if (!setCurrentIndex(newIndex)) {
                updatePageVisibility();
            }
        }
    }

    return true;
}

bool PageViewerWidget::setCurrentIndex(int index) {
    if (index < -1 || index >= static_cast<int>(_pages.size())) {
        RUI_ERROR("Invalid index; index = {}, _pages.size() = {}", index, _pages.size());
        return false;
    }
    if (_currentIndex != index) {
        _currentIndex = index;
        updatePageVisibility();
        currentIndexChanged.emit(_currentIndex);
        return true;
    }

    return false;
}

void PageViewerWidget::updatePageVisibility() {
    for (const WidgetSharedPtr &page : _pages) {
        page->setVisible(false);
    }
    if (_currentIndex >= 0) {
        _pages[_currentIndex]->setVisible(true);
    }
}

} // namespace rendell_ui