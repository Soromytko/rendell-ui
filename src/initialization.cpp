#include <rendell_ui/initialization.h>
#include <ActionPool.h>
#include <GLFW/glfw3.h>
#include "Time.h"
#include "Shaders/ShaderStorage.h"
#include "Widgets/WidgetRegistrator.h"
#include "Widgets/WidgetHandlePipeline.h"
#include "Widgets/WidgetRenderPipeline.h"

namespace rendell_ui
{
	static WidgetRenderPipelineSharedPtr s_widgetRenderPipeline{ nullptr };
	static ActionPoolSharedPtr s_actionPool{ nullptr };

	bool init()
	{
		s_widgetRenderPipeline = makeWidgetRenderPipeline();

		WidgetRegistrator::init({
			makeWidgetHandlePipeline(),
			s_widgetRenderPipeline,
			});

		ShaderStorage::init();

		s_actionPool = makeActionPool();

		return true;
	}

	void release()
	{
		WidgetRegistrator::release();
		ShaderStorage::release();
		s_actionPool = nullptr;
	}

	void draw()
	{
		Time::updateDeltaTime();
		s_widgetRenderPipeline->draw();
	}

	void startFrame()
	{
		s_actionPool->execute();
	}

	void endFrame()
	{

	}

	void callInMainThread(std::function<void()> action)
	{
		s_actionPool->addAction(action);
		glfwPostEmptyEvent();
	}

}

