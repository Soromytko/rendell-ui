#include "rendell_widget.h"
#include "rendell_widget.h"
#include "WidgetRegistrator.h"
#include "WidgetHandlePipeline.h"
#include "WidgetRenderPipeline.h"

namespace rendell_widget
{
	static WidgetRenderPipelineSharedPtr s_widgetRenderPipeline{ nullptr };

	bool init()
	{
		s_widgetRenderPipeline = std::make_shared<WidgetRenderPipeline>();

		WidgetRegistrator::init({
			std::make_shared<WidgetHandlePipeline>(),
			s_widgetRenderPipeline,
			});

		return true;
	}

	void release()
	{
		WidgetRegistrator::release();
	}

	void draw()
	{
		s_widgetRenderPipeline->draw();
	}

}

