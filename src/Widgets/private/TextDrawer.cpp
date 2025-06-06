#include <rendell_ui/Widgets/private/TextDrawer.h>
#include <glm/gtc/matrix_transform.hpp>

namespace rendell_ui
{
	TextDrawer::TextDrawer()
	{

	}

	double TextDrawer::getScrollProgress() const
	{
		return _scrollProgress;
	}

	double TextDrawer::getScrollRatio() const
	{
		return _size.y / static_cast<double>(_textHeight);
	}

	bool TextDrawer::setScrollProgress(double value)
	{
		const double newScrollProgress = _textHeight < _size.y ? 0 : std::clamp(value, 0.0, 1.0);
		if (_scrollProgress != newScrollProgress || true)
		{
			_scrollProgress = newScrollProgress;
			updateScroll();
			return true;
		}
		return false;
	}

	void TextDrawer::onProcessMouseScrolled(glm::dvec2 scroll)
	{
	}

	uint32_t TextDrawer::getTextHeight() const
	{
		return _textHeight;
	}

	glm::dvec2 TextDrawer::getSize() const
	{
		return _size;
	}

	double TextDrawer::getScroll() const
	{
		return _scroll;
	}

	const std::vector<rendell_text::TextRendererSharedPtr>& TextDrawer::getLines() const
	{
		return _textRenderers;
	}

	void TextDrawer::draw(const glm::mat4 viewMatrix, const glm::mat4& modelMatrix) const
	{
		double currentOffset = _size.y * 0.5 + _startRenderingOffset;
		for (size_t i = _startRenderingIndex; i < _textRenderers.size(); i++)
		{
			const rendell_text::TextRendererSharedPtr& textRenderer = _textRenderers[i];
			currentOffset -= static_cast<double>(textRenderer->getTextLayout()->getFontHeight());
			if (currentOffset < -_size.y)
			{
				break;
			}
			const glm::mat4 worldMat = glm::translate(modelMatrix, glm::vec3(-_size.x * 0.5f, currentOffset, 0.0f));
			textRenderer->setMatrix(viewMatrix * worldMat);
			textRenderer->draw();
		}
	}

	void TextDrawer::setSize(glm::dvec2 value)
	{
		if (_size != value)
		{
			_size = value;
			updateScroll();
		}
	}

	void TextDrawer::clear()
	{
		_textRenderers.clear();
		_textHeight = 0;
		_startRenderingIndex = 0;
		_startRenderingOffset = 0.0f;
	}

	void TextDrawer::removeLine(size_t index)
	{
		auto it = _textRenderers.begin() + index;
		_textHeight -= it->operator->()->getTextLayout()->getFontHeight();
		_textRenderers.erase(it);
		optimizeRendering();
	}

	void TextDrawer::addLine(size_t index, const rendell_text::TextRendererSharedPtr& textRenderer)
	{
		_textRenderers.insert(_textRenderers.begin() + index, textRenderer);
		_textHeight += textRenderer->getTextLayout()->getFontHeight();
		optimizeRendering();
	}

	void TextDrawer::swapLines(size_t firstIndex, size_t secondIndex)
	{
		std::swap(_textRenderers[firstIndex], _textRenderers[secondIndex]);
	}

	void TextDrawer::updateScroll()
	{
		_scroll = (static_cast<double>(_textHeight) - _size.y) * _scrollProgress;
		_scroll = std::max(0.0, _scroll);
		optimizeRendering();
	}

	void TextDrawer::optimizeRendering()
	{
		const double baseOffset = _size.y * 0.5;
		double currentOffset = baseOffset + _scroll;
		for (size_t i = 0; i < _textRenderers.size(); i++)
		{
			_startRenderingOffset = currentOffset - baseOffset;
			currentOffset -= _textRenderers[i]->getTextLayout()->getFontHeight();
			if (currentOffset <= _size.y * 0.5f)
			{
				_startRenderingIndex = i;
				return;
			}
		}
	}


}