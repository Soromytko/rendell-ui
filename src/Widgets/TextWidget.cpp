#include "TextWidget.h"
#include "../Viewport.h"

namespace rendell_ui
{
	TextWidget::TextWidget(Widget* parent) : Widget(parent)
	{
		_textRenderer = rendell_text::makeTextRenderer();
	}

	void TextWidget::setFontPath(const std::string& value)
	{
		_textRenderer->setFontPath(value);
	}

	void TextWidget::setFontSize(glm::ivec2 value)
	{
		_textRenderer->setFontSize(value);
	}

	void TextWidget::setBackgroundColor(glm::vec4 value)
	{
		_textRenderer->setBackgroundColor(value);
	}

	void TextWidget::setText(const std::wstring& value)
	{
		_textRenderer->setText(value);
	}

	const std::string& TextWidget::getFontPath() const
	{
		return _textRenderer->getFontPath().string();
	}

	glm::ivec2 TextWidget::getFontSize() const
	{
		return _textRenderer->getFontSize();
	}

	const std::wstring& TextWidget::getText() const
	{
		return _textRenderer->getText();
	}

	void TextWidget::setColor(glm::vec4 value)
	{
		Widget::setColor(value);
		_textRenderer->setColor(value);
	}

	void TextWidget::draw() const
	{
		const glm::mat4& projectMat = Viewport::getCurrent()->getProjectMat();
		const glm::mat4& viewMat = Viewport::getCurrent()->getViewMat();
		const glm::mat4& worldMat = _transform.getMatrix();

		_textRenderer->setMatrix(projectMat * viewMat * worldMat);
		_textRenderer->draw();
	}
}
