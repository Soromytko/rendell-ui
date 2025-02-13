#include "Canvas.h"
#include <glm/gtc/matrix_transform.hpp>

namespace rendell_ui
{
	Canvas::Canvas(ViewportSharedPtr viewport) : WindowEventHandler(), _viewport(viewport)
	{

	}

	void Canvas::addWidget(WidgetSharedPtr widget)
	{
		_widgets.insert(widget);
	}

	void Canvas::removeWidget(WidgetSharedPtr widget)
	{
		_widgets.erase(widget);
	}

	const std::set<WidgetSharedPtr>& Canvas::getWidgets() const
	{
		return _widgets;
	}

	ViewportSharedPtr Canvas::getViewport() const
	{
		return _viewport;
	}

	void Canvas::onRefreshed(int width, int height)
	{
		const float ratio = (float)width / (float)height;
		const float worldWidth = height * ratio;

		const float halfWidth = worldWidth * 0.5f;
		const float halfHeight = height * 0.5f;

		const glm::mat4 projectMat = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
		_viewport->setProjectMat(projectMat);
		_viewport->setParameters(0, 0, width, height);

		for (const WidgetSharedPtr &widget : _widgets)
		{
			widget->updateRecursively();
		}
	}

	void Canvas::onKeyInputted(InputKey key, InputAction action, InputModControl modControln)
	{
		for (const WidgetSharedPtr& widget : _widgets)
		{
			widget->processKeyRecursively(key, action, modControln);
		}
	}

	void Canvas::onCharInputted(unsigned char character)
	{
		for (const WidgetSharedPtr& widget : _widgets)
		{
			widget->processCharRecursively(character);
		}
	}
	
}