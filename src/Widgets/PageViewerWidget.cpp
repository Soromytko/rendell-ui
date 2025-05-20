#include <rendell_ui/Widgets/PageViewerWidget.h>

namespace rendell_ui
{
	const std::vector<WidgetSharedPtr>& PageViewerWidget::getPages() const
	{
		return _pages;
	}

	size_t PageViewerWidget::getIndex() const
	{
		return _currentIndex;
	}

	void PageViewerWidget::setPages(std::vector<WidgetSharedPtr> pages)
	{
		_pages = std::move(pages);
		const auto oldCurrentSignal = _currentIndex;
		_currentIndex = 0;
		updatePageVisibility();
		if (_currentIndex != oldCurrentSignal)
		{
			currentIndexChanged.emit(_currentIndex);
		}
	}

	void PageViewerWidget::addPage(WidgetSharedPtr page)
	{
		_pages.push_back(page);
		updatePageVisibility();
	}

	void PageViewerWidget::setCurrentIndex(int index)
	{
		if (index < -1 || index >= _pages.size())
		{
			std::cerr << "ERROR::PageViewerWidget::setCurrentIndex: Invalid index; index = " <<
				index << ", _pages.size() = " << _pages.size() << std::endl;
			return;
		}
		if (_currentIndex != index)
		{
			_currentIndex = index;
			updatePageVisibility();
			currentIndexChanged.emit(_currentIndex);
		}
	}

	void PageViewerWidget::updatePageVisibility()
	{
		for (const WidgetSharedPtr& page : _pages)
		{
			page->setVisible(false);
		}
		if (_currentIndex >= 0)
		{
			_pages[_currentIndex]->setVisible(true);
		}
	}

}