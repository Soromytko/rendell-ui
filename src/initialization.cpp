#include "initialization.h"
#include "Time.h"
#include "Shaders/ShaderStorage.h"
#include "Widgets/WidgetRegistrator.h"
#include "Widgets/WidgetHandlePipeline.h"
#include "Widgets/WidgetRenderPipeline.h"

namespace rendell_ui
{
	static WidgetRenderPipelineSharedPtr s_widgetRenderPipeline{ nullptr };

	bool init()
	{
		s_widgetRenderPipeline = makeWidgetRenderPipeline();

		WidgetRegistrator::init({
			makeWidgetHandlePipeline(),
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
		Time::updateDeltaTime();
		s_widgetRenderPipeline->draw();
	}

}

