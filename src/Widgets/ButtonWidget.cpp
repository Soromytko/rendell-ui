#include <rendell_ui/Widgets/ButtonWidget.h>

namespace rendell_ui
{
	static const glm::vec3 DEFAULT_COLOR{ 0.5f, 0.0f, 0.0f };
	static const glm::vec3 DEFAULT_HOVER_COLOR = DEFAULT_COLOR * 2.0f;

	static const glm::vec3 CAPTURED_DEFAULT_COLOR{ 0.0f, 0.5f, 0.0f };
	static const glm::vec3 CAPTURED_HOVER_COLOR = CAPTURED_DEFAULT_COLOR * 2.0f;

	ButtonWidget::ButtonWidget() : Widget()
	{
		setColor({ DEFAULT_COLOR, 1.0f });
	}

	void ButtonWidget::draw() const
	{
		_rectangleWidget->draw();
	}

	void ButtonWidget::onSelfWeakPtrChanged()
	{
		if (!_rectangleWidget)
		{
			_rectangleWidget = createUnregisteredRectangleWidget(_selfWeakPtr);
			_rectangleWidget->setAnchor(Anchor::centerStretch);
			_rectangleWidget->setColor(_color);
			_rectangleWidget->setInteract(false);
		}
		else
		{
			_rectangleWidget->setParent(_selfWeakPtr);
		}
	}

	void ButtonWidget::onMouseEntered()
	{
		_isHovered = true;
		if (_isCaptured)
		{
			setColor({ CAPTURED_HOVER_COLOR, 1.0 });
		}
		else
		{
			setColor({ DEFAULT_HOVER_COLOR, 1.0 });
		}
	}

	void ButtonWidget::onMouseExited()
	{
		_isHovered = false;
		if (_isCaptured)
		{
			setColor({ CAPTURED_DEFAULT_COLOR, 1.0 });
		}
		else
		{
			setColor({ DEFAULT_COLOR, 1.0 });
		}
	}

	void ButtonWidget::onCaptured(glm::dvec2 cursorPosition)
	{
		_isCaptured = true;
		setColor({ CAPTURED_DEFAULT_COLOR, 1.0 });
	}

	void ButtonWidget::onFreed(glm::dvec2 cursorPosition)
	{
		_isCaptured = false;
		if (_isHovered)
		{
			setColor({ DEFAULT_HOVER_COLOR, 1.0f });
		}
		else
		{
			setColor({ DEFAULT_COLOR, 1.0f });
		}
	}

	void ButtonWidget::onColorChanged()
	{
		if (_rectangleWidget)
		{
			_rectangleWidget->setColor(_color);
		}
	}

	void ButtonWidget::onMouseClick()
	{
		clicked.emit();
	}

}