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

	void Canvas::focusWidget(Widget* widget)
	{
		setFocusedWidget(widget);
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

		updateWidgetRecursively();
	}

	void Canvas::onKeyInputted(const KeyboardInput& keyboardInput)
	{
		if (_focusedWidget)
		{
			_focusedWidget->onKeyInputted(keyboardInput);
		}
	}

	void Canvas::onMouseButtonInputted(const MouseInput& mouseInput)
	{
		const glm::vec2 cursorPosition = glm::vec2(mouseInput.x, mouseInput.y);
		
		if (mouseInput.action == InputAction::pressed)
		{
			Widget* newFocusedWidget = nullptr;
			for (const WidgetSharedPtr& widget : _widgets)
			{
				Widget* currentFocusedWidget = focusWidgetRecursively(widget.get(), cursorPosition);
				if (currentFocusedWidget)
				{
					newFocusedWidget = currentFocusedWidget;
					break;
				}
			}
			setFocusedWidget(newFocusedWidget);
			if (_focusedWidget)
			{
				_focusedWidget->onMouseDown(cursorPosition);

			}
		}
		else if (mouseInput.action == InputAction::release)
		{
			if (mouseInput.button == InputMouseButton::leftButton)
			{
				if (_focusedWidget && _focusedWidget->intersect(cursorPosition))
				{
					_focusedWidget->onMouseUp(cursorPosition);
				}
			}
		}
	}

	void Canvas::onCharInputted(unsigned char character)
	{
		if (_focusedWidget)
		{
			_focusedWidget->onCharInputted(character);
		}
	}

	void Canvas::setFocusedWidget(Widget* widget)
	{
		if (_focusedWidget != widget)
		{
			if (_focusedWidget)
			{
				_focusedWidget->onUnfocused();
			}
			_focusedWidget = widget;
			_focusedWidget->onFocused();
		}
	}

	void Canvas::updateWidgetRecursively()
	{
		for (const WidgetSharedPtr& widget : _widgets)
		{
			widget->updateRecursively();
		}
	}

	Widget* Canvas::focusWidgetRecursively(Widget* widget, glm::vec2 cursor)
	{
		if (!widget->intersect(cursor))
		{
			return nullptr;
		}

		for (Widget* child : widget->getChildren())
		{
			Widget* childFocusWidget = focusWidgetRecursively(child, cursor);
			if (childFocusWidget)
			{
				return childFocusWidget;
			}
		}

		return widget;
	}

	
}