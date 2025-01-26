#include "TextEdit.h"
#include "../Viewport.h"
#include "private/Cursor.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

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
			height -= _fontSize.y;

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
		}
	}

	void TextEdit::processKey(int key, InputAction action)
	{
		if (action != InputAction::pressed && action != InputAction::repeat)
		{
			return;
		}

		if (key == KEY_LEFT)
		{
			if (!_textEditor->moveCursorToPrevChar() && _currentRowIndex > 0)
			{
				// Use zu to explicitly specify the type of the literal (C++23 and above).
				_currentRowIndex = std::clamp(_currentRowIndex - 1, (size_t)0, _lines.size() - 1);
				_textEditor->setTextRenderer(_lines[_currentRowIndex]);
				_textEditor->moveCursorToEnd();
				_textEditor->_offset.y += _fontSize.y;
				_textEditor->updateRecursively();
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
				_textEditor->_offset.y -= _fontSize.y;
				_textEditor->updateRecursively();
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
				_textEditor->_offset.y -= _fontSize.y;
				_textEditor->updateRecursively();
			}
		}
		else if (key == KEY_TOP)
		{
			if (_currentRowIndex > 0)
			{
				_currentRowIndex = std::clamp(_currentRowIndex - 1, (size_t)0, _lines.size() - 1);
				_textEditor->setTextRenderer(_lines[_currentRowIndex]);
				_textEditor->moveCursorToNearest(_currentColumnIndex);
				_textEditor->_offset.y += _fontSize.y;
				_textEditor->updateRecursively();
			}
		}
	}

}
