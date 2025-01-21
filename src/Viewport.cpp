#include "Viewport.h"
#include <rendell/rendell.h>

static ViewportSharedPtr s_currentViewport{ nullptr };

void Viewport::setCurrent(const ViewportSharedPtr viewport)
{
	s_currentViewport = viewport;
}

ViewportSharedPtr Viewport::getCurrent()
{
	return s_currentViewport;
}

void Viewport::apply()
{
	rendell::setViewport(_x, _y, _width, _height);
}

void Viewport::setParameters(int x, int y, int width, int height)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;
}

void Viewport::setProjectMat(const glm::mat4& value)
{
	_projectMat = value;
}

void Viewport::setViewMat(const glm::mat4& value)
{
	_viewMat = value;
}

const glm::mat4& Viewport::getProjectMat() const
{
	return _projectMat;
}

const glm::mat4& Viewport::getViewMat() const
{
	return _viewMat;
}

glm::ivec2 Viewport::getSize() const
{
	return glm::ivec2({ _width, _height });
}
