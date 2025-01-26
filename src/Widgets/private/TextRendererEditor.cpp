#include "TextRendererEditor.h"
#include <algorithm>

namespace rendell_ui
{
	TextRendererEditor::TextRendererEditor(Widget* parent, rendell_text::TextRendererSharedPtr textRenderer) :
		Widget(parent)
	{
		_cursor = new Cursor(this);
		_cursor->setSize(glm::vec2(2, 24));
		_cursor->setAnchor(Anchor::leftTop);
		setTextRenderer(textRenderer);
	}

	size_t TextRendererEditor::getCursorCharIndex() const
	{
		return _charIndex;
	}

	void TextRendererEditor::setTextRenderer(rendell_text::TextRendererSharedPtr value)
	{
		_textRenderer = value;
		if (!_textRenderer)
		{
			return;
		}
	}

	bool TextRendererEditor::moveCursorToPrevChar()
	{
		if (_charIndex <= 0)
		{
			return false;
		}

		_charIndex--;
		_cursor->moveByDelta(-glm::vec2(_textRenderer->getTextAdvance()[_charIndex], 0.0f));

		return true;
	}

	bool TextRendererEditor::moveCursorToNextChar()
	{
		if (_charIndex >= _textRenderer->getText().length())
		{
			return false;
		}

		_cursor->moveByDelta(glm::vec2(_textRenderer->getTextAdvance()[_charIndex], 0.0f));
		_charIndex++;

		return true;
	}

	bool TextRendererEditor::moveCursorToStart()
	{
		_charIndex = 0;
		_cursor->moveTo(glm::vec2(0.0f, 0.0f));
		return true;
	}

	bool TextRendererEditor::moveCursorToEnd()
	{
		float offset = 0;
		for (_charIndex = 0; _charIndex < _textRenderer->getText().length(); _charIndex++)
		{
			offset += _textRenderer->getTextAdvance()[_charIndex];
		}
		_cursor->moveTo(glm::vec2(offset, 0.0f));
		return true;
	}

	bool TextRendererEditor::moveCursorToNearest(size_t charIndex)
	{
		float offset = 0;
		for (_charIndex = 0; _charIndex != charIndex && _charIndex < _textRenderer->getText().length(); _charIndex++)
		{
			offset += _textRenderer->getTextAdvance()[_charIndex];
		}
		_cursor->moveTo(glm::vec2(offset, 0.0f));

		return true;
	}

}