#include "Text.h"
#include "../Viewport.h"

Text::Text(Widget* parent) : Widget(parent)
{
	_textRenderer = rendell_text::createTextRenderer();
}

void Text::setFontPath(const std::string& value)
{
	_textRenderer->setFontPath(value);
}

void Text::setFontSize(glm::ivec2 value)
{
	_textRenderer->setFontSize(value);
}

void Text::setBackgroundColor(glm::vec4 value)
{
	_textRenderer->setBackgroundColor(value);
}

void Text::setText(const std::wstring& value)
{
	_textRenderer->setText(value);
}

const std::string& Text::getFontPath() const
{
	return _textRenderer->getFontPath().string();
}

glm::ivec2 Text::getFontSize() const
{
	return _textRenderer->getFontSize();
}

const std::wstring& Text::getText() const
{
	return _textRenderer->getText();
}

void Text::setColor(glm::vec4 value)
{
	Widget::setColor(value);
	_textRenderer->setColor(value);
}

void Text::draw() const
{
	const glm::mat4& projectMat = Viewport::getCurrent()->getProjectMat();
	const glm::mat4& viewMat = Viewport::getCurrent()->getViewMat();
	const glm::mat4& worldMat = _transform.getMatrix();

	_textRenderer->setMatrix(projectMat * viewMat * worldMat);
	_textRenderer->draw();
}
