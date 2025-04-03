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

		virtual float getScrollProgress() const = 0;
		virtual float getScrollRatio() const = 0;

		virtual void setScrollProgress(float value) = 0;
	};

	RENDELL_UI_DECLARE_SHARED_PTR(IScrollableWidget)
	RENDELL_UI_DECLARE_WEAK_PTR(IScrollableWidget)
}