#include "TextRendererEditor.h"
#include <algorithm>

#define CURSOR_THICKNESS 2.0f

namespace rendell_ui
{
	TextRendererEditor::TextRendererEditor(Widget* parent, rendell_text::TextRendererSharedPtr textRenderer) :
		Widget(parent)
	{
		_cursor = new Cursor(this);
		_cursor->setAnchor(Anchor::leftStretch);
		_cursor->setThickness(CURSOR_THICKNESS);
		setTextRenderer(textRenderer);
	}

	size_t TextRendererEditor::getCursorCharIndex() const
	{
		return _charIndex;
	}

	void TextRendererEditor::setTextRenderer(rendell_text::TextRendererSharedPtr value)
	{
		if (_textRenderer != value)
		{
			_textRenderer = value;
			updateSize();
		}
	}

	void TextRendererEditor::updateSize(bool shouldCursorOffsetBeRecalculated)
	{
		glm::vec2 size = glm::vec2(0.0f, 0.0f);
		float cursorVerticalOffset = 0.0f;

		if (_textRenderer)
		{
			const rendell_text::GeneralFontMetrices& metrices = _textRenderer->getGeneralFontMetrices();
			size.y = metrices.height;
			// TODO: 0.5 is a magic experimental number.
			cursorVerticalOffset = metrices.descender * 0.5f;
		}

		_cursor->setVerticalOffset(cursorVerticalOffset);
		setSize(size);

		if (shouldCursorOffsetBeRecalculated)
		{
			recalculateCursorOffset();
		}
	}

	bool TextRendererEditor::moveCursorToPrevChar(uint32_t count)
	{
		for (uint32_t i = 0; i < count; i++)
		{
			if (_charIndex <= 0)
			{
				return false;
			}

			_charIndex--;
			_cursor->moveByDelta(-glm::vec2(_textRenderer->getTextAdvance()[_charIndex], 0.0f));
		}

		return true;
	}

	bool TextRendererEditor::moveCursorToNextChar(uint32_t count)
	{
		for (uint32_t i = 0; i < count; i++)
		{
			if (_charIndex >= _textRenderer->getText().length())
			{
				return false;
			}
			_cursor->moveByDelta(glm::vec2(_textRenderer->getTextAdvance()[_charIndex], 0.0f));
			_charIndex++;
		}

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

	bool TextRendererEditor::eraseCursorChar()
	{
		if (_charIndex > 0 && _charIndex <= _textRenderer->getText().length())
		{
			const size_t currentCharIndex = _charIndex - 1;
			moveCursorToPrevChar();
			_textRenderer->eraseChars(currentCharIndex, 1);
			return true;
		}
		return false;
	}

	bool TextRendererEditor::eraseAllAfterCursor()
	{
		_textRenderer->eraseChars(_charIndex, _textRenderer->getText().length() - _charIndex);
		return true;
	}

	bool TextRendererEditor::insertCursorChar(unsigned char character)
	{
		if (_charIndex >= 0 && _charIndex <= _textRenderer->getText().length())
		{
			_textRenderer->insertText(_charIndex, std::wstring{ character });
			moveCursorToNextChar();
			return true;
		}
		return false;
	}

	bool TextRendererEditor::insertAfterCursor(const std::wstring& string)	
	{
		_textRenderer->insertText(_charIndex, string);
		return true;
	}

	void TextRendererEditor::recalculateCursorOffset()
	{
		size_t currentCursor = _charIndex;
		float offset = 0.0f;
		for (_charIndex = 0; _charIndex < currentCursor; _charIndex++)
		{
			offset += _textRenderer->getTextAdvance()[_charIndex];
		}
		_cursor->moveTo(glm::vec2(offset, 0.0f));
	}

}