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
		if (auto locked = _focusedWidget.lock())
		{
			locked->onKeyInputted(keyboardInput);
		}
	}

	void Canvas::onMouseButtonInputted(const MouseInput& mouseInput)
	{
		const glm::vec2 cursorPosition = glm::vec2(mouseInput.x, mouseInput.y);

		if (mouseInput.action == InputAction::pressed)
		{
			const auto hoveredWidgetLocked = _hoveredWidget.lock();
			setFocusedWidget(hoveredWidgetLocked);
			setCapturedWidget(hoveredWidgetLocked, cursorPosition);
			if (auto locked = _focusedWidget.lock())
			{
				locked->onMouseDown(cursorPosition);
			}
			_dragStartPoint = { mouseInput.x, mouseInput.y };
		}
		else if (mouseInput.action == InputAction::release)
		{
			if (mouseInput.button == InputMouseButton::leftButton)
			{
				const WidgetSharedPtr focusedWidgetLocked = _focusedWidget.lock();
				if (focusedWidgetLocked && focusedWidgetLocked->intersect(cursorPosition))
				{
					focusedWidgetLocked->onMouseUp(cursorPosition);
					focusedWidgetLocked->onMouseClick();
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

		if (auto locked = _capturedWidget.lock())
		{
			locked->onDragged(_dragStartPoint, cursor);
		}

		// Emit onMouseExited events.
		for (auto it = _mouseHoverWidgets.begin(); it != _mouseHoverWidgets.end();)
		{
			if (WidgetSharedPtr widget = it->lock())
			{
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
			else
			{
				it = _mouseHoverWidgets.erase(it);
			}
		}

		// Process only the captured widget if it is valid.
		if (auto locked = _capturedWidget.lock())
		{
			hoverMouseRecursively(locked, cursor);
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
		if (_capturedWidget.lock())
		{
			return;
		}

		if (auto locked = _hoveredWidget.lock())
		{
			locked->onMouseScrolled({ x, y });
		}
	}

	void Canvas::onCursorEntered(bool entered)
	{
		if (!entered)
		{
			for (auto it = _mouseHoverWidgets.begin(); it != _mouseHoverWidgets.end(); it++)
			{
				if (auto locked = it->lock())
				{
					locked->onMouseExited();
				}
			}
			_mouseHoverWidgets.clear();
		}
	}

	void Canvas::onCharInputted(unsigned char character)
	{
		if (auto locked = _focusedWidget.lock())
		{
			locked->onCharInputted(character);
		}
	}

	void Canvas::setFocusedWidget(const WidgetSharedPtr& widget)
	{
		auto focusedWidgetLocked = _focusedWidget.lock();

		if (focusedWidgetLocked != widget)
		{
			if (focusedWidgetLocked)
			{
				focusedWidgetLocked->onUnfocused();
			}
			_focusedWidget = widget;
			if (widget)
			{
				widget->onFocused();
			}
		}
	}

	bool Canvas::setCapturedWidget(const WidgetSharedPtr& widget, glm::dvec2 cursorPosition)
	{
		auto capturedWidgetLocked = _capturedWidget.lock();
		if (capturedWidgetLocked != widget)
		{
			if (capturedWidgetLocked)
			{
				capturedWidgetLocked->onFreed(cursorPosition);
			}
			_capturedWidget = widget;
			if (widget)
			{
				widget->onCaptured(cursorPosition);
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

		if (auto it = std::find_if(_mouseHoverWidgets.begin(), _mouseHoverWidgets.end(),
			[=](WidgetWeakPtr currentWidget)
			{
				if (auto locked = currentWidget.lock())
				{
					return locked == widget;
				}
			}) == _mouseHoverWidgets.end())
		{
			_mouseHoverWidgets.push_back(widget);
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