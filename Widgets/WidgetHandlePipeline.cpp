#include "WidgetHandlePipeline.h"
#include <cassert>

void WidgetHandlePipeline::append(IWidget* widget)
{
	assert(widget);
	_widgets.insert(widget);
}

void WidgetHandlePipeline::remove(IWidget* widget)
{
	assert(widget);
	_widgets.erase(widget);
}
