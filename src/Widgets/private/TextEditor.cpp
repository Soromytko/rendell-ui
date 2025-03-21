#include "TextEditor.h"
#include "TextEditorWord.h"
#include "../../String/StringExtension.h"
#include <algorithm>

namespace rendell_ui
{
	static uint32_t s_instanceCount{ 0 };

	static rendell_text::TextLayoutSharedPtr createTextLayout(std::wstring&& text, glm::vec2 fontSize)
	{
		rendell_text::TextLayoutSharedPtr result = rendell_text::makeTextLayout();
		result->setText(text);
		result->setFontPath(FONT_PATH);
		result->setFontSize(fontSize);
		return result;
	}

	TextEditor::TextEditor()
	{
		const rendell_text::TextLayoutSharedPtr textLayout = createTextLayout(L" ", _fontSize);
		addTextLayout(0, textLayout);
		_wordTypes =
		{
			makeDigitTextEditorWord(),
			makeVerbalTextEditorWord(),
			makeTextEditorWord(L" "),
			makeTextEditorWord(L"="),
			makeTextEditorWord(L"("),
			makeTextEditorWord(L")"),
			makeTextEditorWord(L"{"),
			makeTextEditorWord(L"}"),
			makeTextEditorWord(L"<"),
			makeTextEditorWord(L">"),
			makeTextEditorWord(L"~!@#$%^&*_-+;\"\\/,.:"),
		};
	}

	const std::wstring& TextEditor::getText() const
	{
		if (_shouldCachedTextBeUpdated)
		{
			_cachedText = convertLinesToString();
			_shouldCachedTextBeUpdated = false;
		}
		return _cachedText;
	}

	void TextEditor::setText(const std::wstring& value)
	{
		_textLayouts.clear();
		textLayoutCleared.emit();

		std::vector<std::wstring> lines = StringExtension::split(value, L"\n");
		for (std::wstring& line : lines)
		{
			const rendell_text::TextLayoutSharedPtr& textLayout = createTextLayout(std::move(line), _fontSize);
			addTextLayout(_textLayouts.size(), textLayout);
		}

		if (_textLayouts.size() == 0)
		{
			addTextLayout(0, createTextLayout(L"", _fontSize));
		}

		_cachedText = value;
		_shouldCachedTextBeUpdated = false;
	}

	void TextEditor::setFontSize(glm::ivec2 value)
	{
		if (_fontSize != value)
		{
			_fontSize = value;
			for (const rendell_text::TextLayoutSharedPtr& textLayout : _textLayouts)
			{
				textLayout->setFontSize(_fontSize);
			}
		}
	}

	void TextEditor::setWordTypes(const std::vector<TextEditorWordSharedPtr>& value)
	{
		_wordTypes = value;
	}

	static bool isSpaceCharPredicate(wchar_t currentChar)
	{
		return currentChar == L' ';
	}

	size_t TextEditor::getCaretOffsetInString() const
	{
		return _textLayouts[_caret.y]->getTextAdvance()[_caret.x];
	}

	uint32_t TextEditor::getCursorVerticalOffset() const
	{
		uint32_t result = 0;
		for (size_t i = 0; i < _caret.y; i++)
		{
			result += _textLayouts[i]->getFontHeight();
		}
		return result - _textLayouts[_caret.y]->getGeneralFontMetrices().descender / 2;
	}

	uint32_t TextEditor::getCursorHorizontalOffset() const
	{
		if (_caret.x == 0)
		{
			return 0;
		}

		const rendell_text::TextLayoutSharedPtr& textLayout = _textLayouts[_caret.y];
		if (const size_t textLength = textLayout->getTextLength(); _caret.x <= textLength)
		{
			return textLayout->getTextAdvance()[_caret.x - 1];
		}
		return 0;
	}

	uint32_t TextEditor::getCursorHeight() const
	{
		return _textLayouts[_caret.y]->getFontHeight();
	}

	const std::vector<TextEditorWordSharedPtr>& TextEditor::getWordTypes() const
	{
		return _wordTypes;
	}

	bool TextEditor::moveCursorToPrevChar(size_t count)
	{
		if (count == 0)
		{
			return false;
		}

		size_t caretX = _caret.x;
		size_t caretY = _caret.y;
		size_t remainingStepCount = count;
		while (remainingStepCount > 0)
		{
			if (remainingStepCount > caretX)
			{
				remainingStepCount -= caretX;
				if (caretY > 0)
				{
					remainingStepCount--;
					caretY--;
					caretX = _textLayouts[caretY]->getTextLength();
				}
				else
				{
					caretX = 0;
					break;
				}
			}
			else
			{
				caretX -= remainingStepCount;
				remainingStepCount = 0;
				break;
			}
		}
		setCaret(caretX, caretY, true);
		return true;
	}

	bool TextEditor::moveCursorToNextChar(size_t count)
	{
		if (count == 0)
		{
			return false;
		}

		size_t caretX = _caret.x;
		size_t caretY = _caret.y;
		size_t remainingStepCount = count;
		while (remainingStepCount > 0)
		{
			if (const size_t delta = _textLayouts[caretY]->getTextLength() - caretX; remainingStepCount > delta)
			{
				remainingStepCount -= delta;
				if (caretY < _textLayouts.size() - 1)
				{
					remainingStepCount--;
					caretY++;
					caretX = 0;
				}
				else
				{
					caretX = _textLayouts[caretY]->getTextLength();
					break;
				}
			}
			else
			{
				caretX += remainingStepCount;
				remainingStepCount = 0;
				break;
			}
		}
		return setCaret(caretX, caretY, true);
	}

	bool TextEditor::moveCursorToPrevLine(size_t count)
	{
		if (count == 0)
		{
			return false;
		}

		const size_t caretY = count <= _caret.y ? _caret.y - count : 0;
		const size_t caretX = std::min(_caret.xCorrector, _textLayouts[caretY]->getTextLength());
		return setCaret(caretX, caretY);;
	}

	bool TextEditor::moveCursorToNextLine(size_t count)
	{
		if (count == 0)
		{
			return false;
		}

		const size_t caretY = std::min(_caret.y + count, _textLayouts.size() - 1);
		const size_t caretX = std::min(_caret.xCorrector, _textLayouts[caretY]->getTextLength());
		return setCaret(caretX, caretY);
	}

	bool TextEditor::moveCursorToPrevWord()
	{
		return moveCursorToPrevChar(getPrevWordLength());
	}

	bool TextEditor::moveCursorToNextWord()
	{
		return moveCursorToNextChar(getNextWordLength());
	}

	bool TextEditor::moveCursorToStart()
	{
		setCaret(0, _caret.y, true);
		return true;
	}

	bool TextEditor::moveCursorToEnd()
	{
		setCaret(_textLayouts[_caret.y]->getTextLength(), _caret.y, true);
		return true;
	}

	void TextEditor::setupCursorByOffset(double x, double y)
	{
		const size_t caretY = getCaretYByOffset(y);
		const size_t caretX = getCaretXByOffset(caretY, x);
		setCaret(caretX, caretY, true);
	}

	bool TextEditor::eraseBeforeCursor(size_t count)
	{
		if (count == 0 || _caret.x == 0 && _caret.y == 0)
		{
			return false;
		}

		size_t remainingCount = count;
		if (_caret.x >= count)
		{
			_textLayouts[_caret.y]->eraseText(_caret.x - count, count);
			setCaret(_caret.x - count, _caret.y, true);
			_shouldCachedTextBeUpdated = true;
			return true;
		}
		assert(_caret.y > 0);
		remainingCount -= _caret.x;

		const std::wstring& remainingText = takeRemainingTextInLine(_caret.x, _caret.y, false);
		removeTextLayout(_caret.y);
		remainingCount--;
		size_t caretY = _caret.y - 1;
		while (remainingCount > 0 && caretY > 0)
		{
			caretY--;
			if (const size_t textLength = _textLayouts[caretY]->getTextLength(); remainingCount > textLength)
			{
				removeTextLayout(caretY);
				remainingCount -= textLength;
			}
			else
			{
				break;
			}
		}
		const rendell_text::TextLayoutSharedPtr& currentTextLayout = _textLayouts[caretY];
		currentTextLayout->eraseText(currentTextLayout->getTextLength() - remainingCount, remainingCount);
		currentTextLayout->appendText(remainingText);
		setCaret(_textLayouts[caretY]->getTextLength() - remainingText.length(), caretY, true);
		_shouldCachedTextBeUpdated = true;

		return true;
	}

	bool TextEditor::eraseAfterCursor(size_t count)
	{
		if (count == 0)
		{
			return false;
		}

		size_t remainingCount = count;
		const rendell_text::TextLayoutSharedPtr& currentTextLayout = _textLayouts[_caret.y];
		if (const size_t textLength = currentTextLayout->getTextLength(); _caret.x + count < textLength)
		{
			currentTextLayout->eraseText(_caret.x, count);
			remainingCount -= count;
		}
		else if (_caret.x < textLength)
		{
			currentTextLayout->eraseText(_caret.x, count);
			_shouldCachedTextBeUpdated = true;
			return true;
		}

		while (remainingCount > 0 && _caret.y + 1 < _textLayouts.size())
		{
			const rendell_text::TextLayoutSharedPtr removedTextLayout = _textLayouts[_caret.y + 1];
			removeTextLayout(_caret.y + 1);

			remainingCount--;
			if (const size_t textLength = removedTextLayout->getTextLength(); textLength < remainingCount)
			{
				remainingCount -= textLength;
			}
			else
			{
				if (removedTextLayout->getTextLength() > 0)
				{
					const std::wstring& remainingText = removedTextLayout->getSubText(remainingCount);
					_textLayouts[_caret.y]->appendText(remainingText);
				}
				break;
			}
		}
		_shouldCachedTextBeUpdated = true;

		return true;
	}

	bool TextEditor::eraseWordBeforeCursor()
	{
		return eraseBeforeCursor(getPrevWordLength());
	}

	bool TextEditor::eraseWordAfterCursor()
	{
		return eraseAfterCursor(getNextWordLength());
	}

	bool TextEditor::eraseLineUnderCursor()
	{
		if (_textLayouts.size() == 1)
		{
			_textLayouts[0]->eraseText(0);
			setCaret(0, 0);
			_shouldCachedTextBeUpdated = true;
			return true;
		}

		removeTextLayout(_caret.y);
		const size_t caretY = std::min(_caret.y, _textLayouts.size() - 1);
		const size_t caretX = std::min(_caret.xCorrector, _textLayouts[caretY]->getTextLength());
		setCaret(caretX, caretY);
		_shouldCachedTextBeUpdated = true;
		return true;
	}

	bool TextEditor::insertAfterCursor(const std::wstring& text)
	{
		if (text.empty())
		{
			return false;
		}

		std::vector<std::wstring> lines = StringExtension::split(text, L"\n");
		_textLayouts[_caret.y]->insertText(lines[0], _caret.x);

		if (lines.size() == 1)
		{
			setCaret(_caret.x + lines[0].length(), _caret.y);
			_shouldCachedTextBeUpdated = true;
			return true;
		}

		const std::wstring& remainingText = takeRemainingTextInLine(_caret.x + lines[0].length(), _caret.y, true);
		for (size_t i = 1; i < lines.size(); i++)
		{
			const rendell_text::TextLayoutSharedPtr& newTextLayout = createTextLayout(std::move(lines[i]), _fontSize);
			_caret.y++;
			addTextLayout(_caret.y, newTextLayout);
		}
		setCaret(_textLayouts[_caret.y]->getTextLength(), _caret.y);
		_textLayouts[_caret.y]->appendText(remainingText);
		_shouldCachedTextBeUpdated = true;
		return true;
	}

	std::wstring TextEditor::takeRemainingTextInLine(size_t caretX, size_t caretY, bool eraseFromTextLayout)
	{
		const rendell_text::TextLayoutSharedPtr& textLayout = _textLayouts[caretY];
		std::wstring result = textLayout->getTextLength() > caretX ? textLayout->getSubText(caretX) : L"";
		if (eraseFromTextLayout)
		{
			textLayout->eraseText(caretX);
		}
		return result;
	}

	void TextEditor::removeTextLayout(size_t index)
	{
		_textLayouts.erase(_textLayouts.begin() + index);
		textLayoutRemoved.emit(index);
	}

	void TextEditor::addTextLayout(size_t index, const rendell_text::TextLayoutSharedPtr& textLayout)
	{
		_textLayouts.insert(_textLayouts.begin() + index, textLayout);
		textLayoutAdded.emit(index, textLayout);
	}

	bool TextEditor::setCaret(size_t x, size_t y, bool setXCorrector)
	{
		if (setXCorrector)
		{
			_caret.xCorrector = x;
		}

		if (_caret.x != x || _caret.y != y)
		{
			_caret.x = x;
			_caret.y = y;
			cursorChanged.emit(getCursorHorizontalOffset(), getCursorVerticalOffset(), getCursorHeight());
			return true;
		}
		return false;
	}

	size_t TextEditor::getPrevWordLength() const
	{
		size_t result = 0;
		size_t caretX = _caret.x;
		size_t caretY = _caret.y;
		if (caretX == 0 && caretY > 0)
		{
			caretY--;
			caretX = _textLayouts[caretY]->getTextLength();
			result++;
		}

		if (const std::wstring& text = _textLayouts[caretY]->getText(); text.length() > 0)
		{
			size_t i = caretX;
			if (i > 0)
			{
				if (const TextEditorWordSharedPtr& word = findWord(text[i - 1]); word != nullptr)
				{
					while (i > 0 && word->isWordCharacter(text[i - 1]))
					{
						result++;
						i--;
					}
				}
			}
		}
		return result;

	}

	size_t TextEditor::getNextWordLength() const
	{
		size_t result = 0;
		size_t caretX = _caret.x;
		size_t caretY = _caret.y;
		if (caretX == _textLayouts[caretY]->getTextLength() && caretY < _textLayouts.size() - 1)
		{
			caretY++;
			caretX = 0;
			result++;
		}

		if (const std::wstring& text = _textLayouts[caretY]->getText(); text.size() > 0)
		{
			size_t i = caretX;
			if (const TextEditorWordSharedPtr& word = findWord(text[i]); word != nullptr)
			{
				while (i < text.length() && word->isWordCharacter(text[i]))
				{
					result++;
					i++;
				}
			}
		}
		return result;
	}

	size_t TextEditor::getCaretYByOffset(double offset) const
	{
		assert(_textLayouts.size() > 0);

		size_t currentOffset = _textLayouts[0]->getFontHeight() / 2;
		double distance = abs(offset - static_cast<double>(currentOffset));
		for (size_t i = 0; i < _textLayouts.size(); i++)
		{
			currentOffset += _textLayouts[i]->getFontHeight();
			double newDistance = abs(offset - static_cast<double>(currentOffset));
			if (newDistance < distance)
			{
				distance = newDistance;
			}
			else
			{
				return i;
			}
		}

		return _textLayouts.size() - 1;
	}

	size_t TextEditor::getCaretXByOffset(size_t caretY, double offset) const
	{
		const std::vector<uint32_t>& textAdvance = _textLayouts[caretY]->getTextAdvance();
		if (textAdvance.size() == 0)
		{
			return 0;
		}

		size_t currentOffset = textAdvance[0] / 2;
		double distance = abs(offset - static_cast<double>(currentOffset));
		for (size_t i = 0; i < textAdvance.size(); i++)
		{
			currentOffset = textAdvance[i];
			double newDistance = abs(offset - static_cast<double>(currentOffset));
			if (newDistance < distance)
			{
				distance = newDistance;
			}
			else
			{
				return i;
			}
		}

		return textAdvance.size();
	}

	const TextEditorWordSharedPtr& TextEditor::findWord(wchar_t character) const
	{
		for (const TextEditorWordSharedPtr& word : _wordTypes)
		{
			if (word->isWordCharacter(character))
			{
				return word;
			}
		}
		return nullptr;
	}

	std::wstring TextEditor::convertLinesToString() const
	{
		size_t resultLength = 0;
		for (const rendell_text::TextLayoutSharedPtr& textLayout : _textLayouts)
		{
			resultLength += textLayout->getText().length() + 1;
		}

		std::wstring result;
		result.reserve(resultLength);
		for (const rendell_text::TextLayoutSharedPtr& textLayout : _textLayouts)
		{
			result += textLayout->getText() + L"\n";
		}

		return result;
	}

}