#include "ButtonWidget.h"

namespace rendell_ui
{
	static const glm::vec3 DEFAULT_COLOR{ 0.5f, 0.0f, 0.0f };
	static const glm::vec3 DEFAULT_HOVER_COLOR = DEFAULT_COLOR * 2.0f;

	static const glm::vec3 CAPTURED_DEFAULT_COLOR{ 0.0f, 0.5f, 0.0f };
	static const glm::vec3 CAPTURED_HOVER_COLOR = CAPTURED_DEFAULT_COLOR * 2.0f;
	//static const glm::vec3 CAPTURED_COLOR = DEFAULT_COLOR * 1.5f;

	void ButtonWidget::onFocused()
	{

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

	void ButtonWidget::onMouseDown(glm::dvec2 cursorPosition)
	{
	}

	void ButtonWidget::onMouseUp(glm::dvec2 cursorPosition)
	{
	}

	void ButtonWidget::onCaptured()
	{
		_isCaptured = true;
		setColor({ CAPTURED_DEFAULT_COLOR, 1.0 });
	}

	void ButtonWidget::onFreed()
	{
		_isCaptured = false;
		//setColor({ 1.0, 0.0, 0.0, 1.0 });
		if (_isHovered)
		{
			setColor({ DEFAULT_HOVER_COLOR, 1.0f });
		}
		else
		{
			setColor({ DEFAULT_COLOR, 1.0f });
		}

	}

	void ButtonWidget::onMouseClick()
	{
		std::cout << "Clicked" << std::endl;
	}
}