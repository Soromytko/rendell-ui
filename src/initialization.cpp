#include "initialization.h"
#include "Shaders/ShaderStorage.h"
#include "Widgets/WidgetRegistrator.h"
#include "Widgets/WidgetHandlePipeline.h"
#include "Widgets/WidgetRenderPipeline.h"

namespace rendell_ui
{
	static WidgetRenderPipelineSharedPtr s_widgetRenderPipeline{ nullptr };

	bool init()
	{
		s_widgetRenderPipeline = std::make_shared<WidgetRenderPipeline>();

		WidgetRegistrator::init({
			std::make_shared<WidgetHandlePipeline>(),
			s_widgetRenderPipeline,
			});

		ShaderStorage::init();

		return true;
	}

	void release()
	{
		WidgetRegistrator::release();
		ShaderStorage::release();
	}

	void draw()
	{
		s_widgetRenderPipeline->draw();
	}

}

