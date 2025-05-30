#include "WidgetRenderPipeline.h"
#include <cassert>
#include <algorithm>

namespace rendell_ui
{
	void WidgetRenderPipeline::append(IWidget* widget)
	{
		assert(widget);
		_widgets.push_back(widget);
	}

	void WidgetRenderPipeline::remove(IWidget* widget)
	{
		assert(widget);
		auto newEnd = std::remove(_widgets.begin(), _widgets.end(), widget);
		_widgets.erase(newEnd, _widgets.end());
	}

	void WidgetRenderPipeline::draw() const
	{
		for (IWidget* widget : _widgets)
		{
			if (widget->getImplicitVisible())
			{
				widget->draw();
			}
		}

	}
}
