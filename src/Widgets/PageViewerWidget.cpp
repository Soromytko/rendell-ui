#include <rendell_ui/Widgets/PageViewerWidget.h>

namespace rendell_ui
{
	const std::vector<WidgetSharedPtr>& PageViewerWidget::getPages() const
	{
		return _pages;
	}

	size_t PageViewerWidget::getCurrentIndex() const
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

	bool PageViewerWidget::removePage(WidgetSharedPtr page)
	{
		auto it = std::find(_pages.begin(), _pages.end(), page);
		if (it == _pages.end())
		{
			std::cerr << "ERROR::PageViewerWidget::removePage: This is not the Page Widget " << page << std::endl;
			return false;
		}

		_pages.erase(it);
		return true;
	}

	bool PageViewerWidget::removePage(size_t pageIndex)
	{
		if (pageIndex >= _pages.size())
		{
			std::cerr << "ERROR::PageViewerWidget::removePage: Invalid index; index = "
				<< pageIndex << " _page.size() = " << _pages.size() << std::endl;
			return false;
		}

		_pages.erase(_pages.begin() + pageIndex);
		return true;
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