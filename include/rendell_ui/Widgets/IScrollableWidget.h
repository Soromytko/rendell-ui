#pragma once
#include <rendell_ui/defines.h>

namespace rendell_ui
{
	class IScrollableWidget
	{
	protected:
		IScrollableWidget() = default;
	public:
		~IScrollableWidget() = default;

		virtual double getScrollProgress() const = 0;
		virtual double getScrollRatio() const = 0;

		virtual bool setScrollProgress(double value) = 0;
		virtual void onProcessMouseScrolled(glm::dvec2 scroll) = 0;
	};

	RENDELL_UI_DECLARE_SHARED_PTR(IScrollableWidget)
	RENDELL_UI_DECLARE_WEAK_PTR(IScrollableWidget)
}