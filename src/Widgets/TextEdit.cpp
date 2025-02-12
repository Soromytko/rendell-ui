#include "TextEdit.h"
#include "../Viewport.h"
#include "private/Cursor.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#define KEY_ENTER 257
#define KEY_TAB 258
#define KEY_BACKSPACE 259
#define KEY_DELETE 261
#define KEY_RIGHT 262
#define KEY_LEFT 263
#define KEY_BOTTOM 264
#define KEY_TOP 265

namespace rendell_ui
{
	static rendell_text::TextRendererSharedPtr createTextRenderer(std::wstring&& text, glm::vec2 fontSize)
	{
		rendell_text::TextRendererSharedPtr result = rendell_text::createTextRenderer();
		result->setText(text);
		result->setFontPath("../res/Fonts/mononoki/mononoki-Regular.ttf");
		result->setFontSize(fontSize);
		result->setBackgroundColor(glm::vec4(31.0f / 255.0, 31.0f / 255.0, 31.0f / 255.0, 1.0f));
		result->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		return result;
	}

	TextEdit::TextEdit(Widget* parent) : Widget(parent)
	{
		_textEditor = new TextRendererEditor(this);
		_textEditor->setAnchor(Anchor::leftTop);
		_textEditor->updateSize();
	}

	void TextEdit::draw() const
	{
		const glm::mat4& projectMat = Viewport::getCurrent()->getProjectMat();
		const glm::mat4& viewMat = Viewport::getCurrent()->getViewMat();
		const glm::mat4& transformMat = _transform.getMatrix();
		float height = _size.y * 0.5f - _fontSize.y;

		for (const rendell_text::TextRendererSharedPtr& line : _lines)
		{
			const glm::mat4 worldMat = glm::translate(transformMat, glm::vec3(-_size.x * 0.5f, height, 0.0f));
			height -= line->getGeneralFontMetrices().height;

			line->setMatrix(projectMat * viewMat * worldMat);
			line->draw();
		}
	}

	void TextEdit::setText(const std::wstring& value)
	{
		_lines.clear();

		std::wstring line;
		for (auto it = value.begin(); it != value.end(); it++)
		{
			if (wchar_t currentChar = *it; currentChar != '\n')
			{
				line += currentChar;
			}
			else
			{
				_lines.push_back(createTextRenderer(std::move(line), _fontSize));
				line.clear();
			}
		}
		_textEditor->setTextRenderer(_lines[0]);

	}

	void TextEdit::setFontSize(glm::ivec2 value)
	{
		if (value != _fontSize)
		{
			_fontSize = value;
			for (rendell_text::TextRendererSharedPtr& line : _lines)
			{
				line->setFontSize(_fontSize);
			}
			_textEditor->updateSize(true);
		}
	}

	void TextEdit::setupTextEditor()
	{
		float offset = 0.0f;
		for (size_t row = 0; row < _currentRowIndex; row++)
		{
			rendell_text::TextRendererSharedPtr& line = _lines[row];
			offset += static_cast<float>(line->getGeneralFontMetrices().height);
		}
		_textEditor->setOffset(glm::vec2(0.0, -offset));
		_textEditor->updateRecursively();
	}

	void TextEdit::processKey(int key, InputAction action, InputModControl modControl)
	{
		if (action != InputAction::pressed && action != InputAction::repeat)
		{
			return;
		}

		if (modControl.hasCtrlMod())
		{
			processKeyWithCtrl(key);
			return;
		}
		else if (modControl.hasShiftMod())
		{
			if (key == KEY_DELETE)
			{
				if (_lines.size() > 1)
				{
					_lines.erase(_lines.begin() + _currentRowIndex);
					if (_currentRowIndex == _lines.size())
					{
						_currentRowIndex--;
					}
					_textEditor->setTextRenderer(_lines[_currentRowIndex]);
					setupTextEditor();
					_textEditor->moveCursorToNearest(_currentColumnIndex);
				}
				else if (_lines.size() == 1)
				{
					_lines[_currentRowIndex]->setText(L"");
					_textEditor->moveCursorToStart();
				}
				_currentColumnIndex = _textEditor->getCursorCharIndex();
				return;

			}
		}

		if (key == KEY_LEFT)
		{
			if (!_textEditor->moveCursorToPrevChar() && _currentRowIndex > 0)
			{
				// Use zu to explicitly specify the type of the literal (C++23 and above).
				_currentRowIndex = std::clamp(_currentRowIndex - 1, (size_t)0, _lines.size() - 1);
				_textEditor->setTextRenderer(_lines[_currentRowIndex]);
				_textEditor->moveCursorToEnd();
				setupTextEditor();
			}
			_currentColumnIndex = _textEditor->getCursorCharIndex();
		}
		else if (key == KEY_RIGHT)
		{
			if (!_textEditor->moveCursorToNextChar() && _currentRowIndex < _lines.size() - 1)
			{
				_currentRowIndex = std::clamp(_currentRowIndex + 1, (size_t)0, _lines.size() - 1);
				_textEditor->setTextRenderer(_lines[_currentRowIndex]);
				_textEditor->moveCursorToStart();
				setupTextEditor();
			}
			_currentColumnIndex = _textEditor->getCursorCharIndex();
		}
		else if (key == KEY_BOTTOM)
		{
			if (_currentRowIndex < _lines.size() - 1)
			{
				_currentRowIndex = std::clamp(_currentRowIndex + 1, (size_t)0, _lines.size() - 1);
				_textEditor->setTextRenderer(_lines[_currentRowIndex]);
				_textEditor->moveCursorToNearest(_currentColumnIndex);
				setupTextEditor();
			}
		}
		else if (key == KEY_TOP)
		{
			if (_currentRowIndex > 0)
			{
				_currentRowIndex = std::clamp(_currentRowIndex - 1, (size_t)0, _lines.size() - 1);
				_textEditor->setTextRenderer(_lines[_currentRowIndex]);
				_textEditor->moveCursorToNearest(_currentColumnIndex);
				setupTextEditor();
			}
		}
		else if (key == KEY_ENTER)
		{
			std::wstring remaningText{};
			remaningText.insert(0, _lines[_currentRowIndex]->getText(),
				_currentColumnIndex, _lines[_currentRowIndex]->getText().length() - _currentColumnIndex);
			_currentRowIndex++;
			_lines.insert(_lines.begin() + _currentRowIndex, createTextRenderer(std::move(remaningText), _fontSize));
			_textEditor->eraseAllAfterCursor();
			_textEditor->setTextRenderer(_lines[_currentRowIndex]);
			_textEditor->moveCursorToStart();
			setupTextEditor();
			_currentColumnIndex = _textEditor->getCursorCharIndex();
		}
		else if (key == KEY_TAB)
		{
			static const std::wstring tabString = L"    ";
			_textEditor->insertAfterCursor(tabString);
			_textEditor->moveCursorToNextChar(tabString.length());
			_currentColumnIndex = _textEditor->getCursorCharIndex();
		}
		else if (key == KEY_BACKSPACE)
		{
			if (!_textEditor->eraseCursorChar() && _currentRowIndex > 0)
			{
				std::wstring remaningText = _lines[_currentRowIndex]->getText();
				_lines.erase(_lines.begin() + _currentRowIndex);
				_currentRowIndex = std::clamp(_currentRowIndex - 1, (size_t)0, _lines.size() - 1);
				_textEditor->setTextRenderer(_lines[_currentRowIndex]);
				_textEditor->moveCursorToEnd();
				_textEditor->insertAfterCursor(remaningText);
				setupTextEditor();
			}
			_currentColumnIndex = _textEditor->getCursorCharIndex();
		}
		else if (key == KEY_DELETE)
		{
			// TODO: Copy paste is here, refactoring is required.
			if (!_textEditor->moveCursorToNextChar() && _currentRowIndex < _lines.size() - 1)
			{
				_currentRowIndex = std::clamp(_currentRowIndex + 1, (size_t)0, _lines.size() - 1);
				_textEditor->setTextRenderer(_lines[_currentRowIndex]);
				_textEditor->moveCursorToStart();
				setupTextEditor();
			}
			_currentColumnIndex = _textEditor->getCursorCharIndex();
			if (!_textEditor->eraseCursorChar() && _currentRowIndex > 0)
			{
				std::wstring remaningText = _lines[_currentRowIndex]->getText();
				_lines.erase(_lines.begin() + _currentRowIndex);
				_currentRowIndex = std::clamp(_currentRowIndex - 1, (size_t)0, _lines.size() - 1);
				_textEditor->setTextRenderer(_lines[_currentRowIndex]);
				_textEditor->moveCursorToEnd();
				_textEditor->insertAfterCursor(remaningText);
				setupTextEditor();
			}
			_currentColumnIndex = _textEditor->getCursorCharIndex();
		}
	}

	void TextEdit::processChar(unsigned char character)
	{
		_textEditor->insertCursorChar(character);
		_currentColumnIndex = _textEditor->getCursorCharIndex();
	}

	void TextEdit::processKeyWithCtrl(int key)
	{
		if (key == KEY_LEFT)
		{
			_textEditor->moveCursorToPrevWord();
			_currentColumnIndex = _textEditor->getCursorCharIndex();
		}
		else if (key == KEY_RIGHT)
		{
			_textEditor->moveCursorToNextWord();
			_currentColumnIndex = _textEditor->getCursorCharIndex();
		}
		else if (key == KEY_BACKSPACE)
		{
			_textEditor->eraseWordBeforeCursor();
			_currentColumnIndex = _textEditor->getCursorCharIndex();
		}
		else if (key == KEY_DELETE)
		{
			if (!_textEditor->eraseWordAfterCursor() && _currentRowIndex + 1 < _lines.size())
			{
				_lines[_currentRowIndex]->setText(_lines[_currentRowIndex]->getText() + _lines[_currentRowIndex + 1]->getText());
				_lines.erase(_lines.begin() + _currentRowIndex + 1);
				_textEditor->eraseWordAfterCursor();
			}
		}
	}

}
