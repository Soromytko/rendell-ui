#include <rendell_ui/Canvas.h>
#include <glm/gtc/matrix_transform.hpp>

namespace rendell_ui
{
	Canvas::Canvas(ViewportSharedPtr viewport) : WindowEventHandler(), _viewport(viewport)
	{

	}

	void Canvas::addWidget(const WidgetSharedPtr& widget)
	{
		_widgets.insert(widget);
	}

	void Canvas::removeWidget(const WidgetSharedPtr& widget)
	{
		_widgets.erase(widget);
	}

	void Canvas::focusWidget(const WidgetSharedPtr& widget)
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
			setFocusedWidget(_hoveredWidget);
			setCapturedWidget(_hoveredWidget, cursorPosition);
			if (_focusedWidget)
			{
				_focusedWidget->onMouseDown(cursorPosition);
			}
			_dragStartPoint = { mouseInput.x, mouseInput.y };
		}
		else if (mouseInput.action == InputAction::release)
		{
			if (mouseInput.button == InputMouseButton::leftButton)
			{
				if (_focusedWidget && _focusedWidget->intersect(cursorPosition))
				{
					_focusedWidget->onMouseUp(cursorPosition);
					_focusedWidget->onMouseClick();
				}

				// Handle mouse movement to update widgets currently under the cursor.
				if (setCapturedWidget(nullptr))
				{
					onMouseMoved(mouseInput.x, mouseInput.y);
				}
			}
		}
	}

	void Canvas::onMouseMoved(double x, double y)
	{
		const glm::dvec2 cursor{ x, y };

		if (_capturedWidget)
		{
			_capturedWidget->onDragged(_dragStartPoint, cursor);
		}

		// Emit onMouseExited events.
		for (auto it = _mouseHoverWidgets.begin(); it != _mouseHoverWidgets.end();)
		{
			WidgetSharedPtr widget = *it;
			if (!widget->intersect(cursor))
			{
				it = _mouseHoverWidgets.erase(it);
				widget->onMouseExited();
			}
			else
			{
				it++;
			}
		}

		// Process only the captured widget if it is valid.
		if (_capturedWidget)
		{
			hoverMouseRecursively(_capturedWidget, cursor);
		}
		else
		{
			for (const WidgetSharedPtr& widget : _widgets)
			{
				_hoveredWidget = hoverMouseRecursively(widget, cursor);
			}
		}
	}

	void Canvas::onMouseScrolled(double x, double y)
	{
		if (_capturedWidget)
		{
			return;
		}

		if (_hoveredWidget)
		{
			_hoveredWidget->onMouseScrolled({ x, y });
		}
	}

	void Canvas::onCharInputted(unsigned char character)
	{
		if (_focusedWidget)
		{
			_focusedWidget->onCharInputted(character);
		}
	}

	void Canvas::setFocusedWidget(const WidgetSharedPtr& widget)
	{
		if (_focusedWidget != widget)
		{
			if (_focusedWidget)
			{
				_focusedWidget->onUnfocused();
			}
			_focusedWidget = widget;
			if (_focusedWidget)
			{
				_focusedWidget->onFocused();
			}
		}
	}

	bool Canvas::setCapturedWidget(const WidgetSharedPtr& widget, glm::dvec2 cursorPosition)
	{
		if (_capturedWidget != widget)
		{
			if (_capturedWidget)
			{
				_capturedWidget->onFreed(cursorPosition);
			}
			_capturedWidget = widget;
			if (_capturedWidget)
			{
				_capturedWidget->onCaptured(cursorPosition);
			}
			return true;
		}
		return false;
	}

	void Canvas::updateWidgetRecursively()
	{
		for (const WidgetSharedPtr& widget : _widgets)
		{
			widget->updateRecursively();
		}
	}

	WidgetSharedPtr Canvas::hoverMouseRecursively(const WidgetSharedPtr& widget, glm::dvec2 cursor)
	{
		if (!widget->getVisible() || !widget->getInteract() || !widget->intersect(cursor))
		{
			return nullptr;
		}

		if (_mouseHoverWidgets.find(widget) == _mouseHoverWidgets.end())
		{
			_mouseHoverWidgets.insert(widget);
			widget->onMouseEntered();
		}

		widget->onMouseHovered(cursor);

		for (const WidgetSharedPtr& child : widget->getChildren())
		{
			if (const WidgetSharedPtr& childHoverWidget = hoverMouseRecursively(child, cursor))
			{
				return childHoverWidget;
			}
		}
		return widget;
	}

}