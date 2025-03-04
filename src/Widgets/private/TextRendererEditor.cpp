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
		_cursor->setVisible(false);
		setTextRenderer(textRenderer);
	}

	size_t TextRendererEditor::getCursorCharIndex() const
	{
		return _charIndex;
	}

	rendell_text::TextRendererSharedPtr TextRendererEditor::getTextRenderer() const
	{
		return _textRenderer;
	}

	void TextRendererEditor::setTextRenderer(rendell_text::TextRendererSharedPtr value)
	{
		if (_textRenderer != value)
		{
			_textRenderer = value;
			updateSize();

			_cursor->setVisible(_textRenderer != nullptr);
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

	static bool isSpaceCharPredicate(wchar_t currentChar)
	{
		return currentChar == L' ';
	}

	bool TextRendererEditor::moveCursorToPrevWord()
	{
		if (moveCursorToPrevUntil(L" ")) {
			moveCursorToPrevUntil(isSpaceCharPredicate);
			return true;
		}
		moveCursorToPrevUntil(isSpaceCharPredicate);

		return false;
	}

	bool TextRendererEditor::moveCursorToNextWord()
	{
		if (moveCursorToNextUntil(L" ")) {
			moveCursorToNextUntil(isSpaceCharPredicate);
			return true;
		}
		moveCursorToNextUntil(isSpaceCharPredicate);

		return false;
	}

	bool TextRendererEditor::moveCursorToPrevUntil(std::function<bool(wchar_t currentChar)> predicate)
	{
		const std::wstring& text = _textRenderer->getText();
		const std::vector<uint32_t>& textAdvanced = _textRenderer->getTextAdvance();
		uint32_t cursorDelta = 0;
		for (uint32_t i = cursorDelta; i >= 0 && _charIndex > 0; i--)
		{
			_charIndex--;
			if (!predicate(text[_charIndex]))
			{
				_charIndex++;
				break;
			}
			cursorDelta += textAdvanced[_charIndex];
		}

		if (cursorDelta != 0)
		{
			_cursor->moveByDelta(-glm::vec2(cursorDelta, 0.0f));
			return true;
		}

		return false;
	}

	bool TextRendererEditor::moveCursorToNextUntil(std::function<bool(wchar_t currentChar)> predicate)
	{
		const std::wstring& text = _textRenderer->getText();
		const std::vector<uint32_t>& textAdvanced = _textRenderer->getTextAdvance();
		uint32_t cursorDelta = 0;
		for (uint32_t i = _charIndex; i < text.length(); i++)
		{
			if (!predicate(text[_charIndex]))
			{
				break;
			}
			cursorDelta += textAdvanced[_charIndex];
			_charIndex++;
		}

		if (cursorDelta > 0)
		{
			_cursor->moveByDelta(glm::vec2(cursorDelta, 0.0f));
			return true;
		}

		return false;
	}

	bool TextRendererEditor::moveCursorToNextUntil(const std::wstring& breakingCharacters)
	{
		return moveCursorToNextUntil([&breakingCharacters](wchar_t currentChar) -> bool
			{
				return breakingCharacters.find(currentChar) == std::wstring::npos;
			});
	}

	bool TextRendererEditor::moveCursorToPrevUntil(const std::wstring& breakingCharacters)
	{
		return moveCursorToPrevUntil([&breakingCharacters](wchar_t currentChar) -> bool
			{
				return breakingCharacters.find(currentChar) == std::wstring::npos;
			});
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

	void TextRendererEditor::setupCursorByOffset(double offset)
	{
		const std::vector<uint32_t>& textAdvance = _textRenderer->getTextAdvance();

		double accOffset = 0.0f;
		double distance = abs(offset - accOffset);
		for (int64_t i = 0; i < textAdvance.size(); i++)
		{
			accOffset += textAdvance[i];
			double newDistance = abs(offset - accOffset);
			if (newDistance < distance)
			{
				distance = newDistance;
			}
			else
			{
				_cursor->moveTo(glm::vec2(accOffset - textAdvance[i], 0.0f));
				_charIndex = i;
				return;
			}
		}

		moveCursorToEnd();
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

	bool TextRendererEditor::eraseWordBeforeCursor()
	{
		const size_t endIndex = _charIndex;
		if (moveCursorToPrevWord())
		{
			const size_t startIndex = _charIndex;
			const size_t count = endIndex - startIndex;
			_textRenderer->eraseChars(startIndex, count);
			return true;
		}
		return false;
	}

	bool TextRendererEditor::eraseWordAfterCursor()
	{
		const size_t startIndex = _charIndex;
		if (moveCursorToNextWord())
		{
			const size_t endIndex = _charIndex;
			const size_t count = endIndex - startIndex;
			moveCursorToPrevChar(count);
			_textRenderer->eraseChars(startIndex, count);
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