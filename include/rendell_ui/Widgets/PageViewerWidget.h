#pragma once
#include <vector>
#include "Widget.h"
#include "../Signal.h"

namespace rendell_ui
{
	class PageViewerWidget final : public Widget
	{
	public:
		PageViewerWidget() = default;
		~PageViewerWidget() = default;

		const std::vector<WidgetSharedPtr>& getPages() const;
		size_t getIndex() const;

		void setPages(std::vector<WidgetSharedPtr> pages);
		void addPage(WidgetSharedPtr page);
		void setCurrentIndex(int index);

		Signal<void, size_t> currentIndexChanged;

	private:
		void updatePageVisibility();

		std::vector<WidgetSharedPtr> _pages;
		int _currentIndex{ -1 };

	};

	RENDELL_UI_DECLARE_WIDGET(PageViewerWidget)
}
