#include "TextEditWidget.h"
#include "../Viewport.h"
#include "private/Cursor.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace rendell_ui
{
	static rendell_text::TextRendererSharedPtr createTextRenderer(std::wstring&& text, glm::vec2 fontSize)
	{
		rendell_text::TextRendererSharedPtr result = rendell_text::makeTextRenderer();
		result->setText(text);
		result->setFontPath(FONT_PATH);
		result->setFontSize(fontSize);
		result->setBackgroundColor(glm::vec4(31.0f / 255.0, 31.0f / 255.0, 31.0f / 255.0, 1.0f));
		result->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		return result;
	}

	TextEditWidget::TextEditWidget(Widget* parent) : Widget(parent)
	{
		_textEditor = new TextRendererEditor(this);
		_textEditor->setVisible(false);
		_textEditor->setAnchor(Anchor::leftTop);
		_textEditor->updateSize();
	}

	void TextEditWidget::draw() const
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

	static std::vector<std::wstring> split_string(const std::wstring& str, const std::wstring& delimiter)
	{
		std::vector<std::wstring> result;

		std::wstring::size_type pos = 0;
		std::wstring::size_type prev = 0;
		while ((pos = str.find(delimiter, prev)) != std::wstring::npos)
		{
			result.push_back(str.substr(prev, pos - prev));
			prev = pos + delimiter.length();
		}

		result.push_back(str.substr(prev));

		return result;
	}

	void TextEditWidget::setText(const std::wstring& value)
	{
		_lines.clear();

		std::vector<std::wstring> lines = split_string(value, L"\n");
		for (std::wstring& line : lines)
		{
			_lines.push_back(createTextRenderer(std::move(line), _fontSize));
		}

		if (_lines.size() > 0)
		{
			_textEditor->setTextRenderer(_lines[0]);
			_textEditor->moveCursorToStart();
			_currentRowIndex = 0;
			_currentColumnIndex = 0;
			setupTextEditor();
		}
		else
		{
			_textEditor->setTextRenderer(nullptr);
		}
	}

	void TextEditWidget::setFontSize(glm::ivec2 value)
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

	void TextEditWidget::setupTextEditor()
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

	void TextEditWidget::onFocused()
	{
		_textEditor->setVisible(true);
		if (!_textEditor->getTextRenderer() && _lines.size() > 0)
		{
			_textEditor->setTextRenderer(_lines[0]);
		}
	}

	void TextEditWidget::onUnfocused()
	{
		_textEditor->setVisible(false);
	}

	void TextEditWidget::onMouseDown(glm::dvec2 cursorPosition)
	{
		if (_lines.size() == 0)
		{
			return;
		}

		cursorPosition = static_cast<glm::dvec2>(_size * 0.5f) -
			cursorPosition - static_cast<glm::dvec2>(_transform.getPosition());

		double offset = _lines[0]->getGeneralFontMetrices().height * 0.5f;
		double distance = abs(cursorPosition.y - _lines[0]->getGeneralFontMetrices().height * 0.5f);
		for (size_t row = 0; row < _lines.size(); row++)
		{
			offset += _lines[row]->getGeneralFontMetrices().height;
			double currentDistance = abs(cursorPosition.y - offset);
			if (currentDistance < distance)
			{
				distance = currentDistance;
			}
			else
			{
				_currentRowIndex = row;
				_textEditor->setTextRenderer(_lines[row]);
				setupTextEditor();
				break;
			}
		}

		_textEditor->setupCursorByOffset(cursorPosition.x);
		_currentColumnIndex = _textEditor->getCursorCharIndex();
	}

	void TextEditWidget::onKeyInputted(const KeyboardInput& keyboardInput)
	{
		if (keyboardInput.action != InputAction::pressed && keyboardInput.action != InputAction::repeat)
		{
			return;
		}

		switch (keyboardInput.key)
		{
		case InputKey::enter: processKeyEnter(keyboardInput.modControl); return;
		case InputKey::tab: processKeyTab(keyboardInput.modControl); return;
		case InputKey::backspace: processKeyBackspace(keyboardInput.modControl); return;
		case InputKey::del: processKeyDelete(keyboardInput.modControl); return;
		case InputKey::right: processKeyRight(keyboardInput.modControl); return;
		case InputKey::left: processKeyLeft(keyboardInput.modControl); return;
		case InputKey::down: processKeyDown(keyboardInput.modControl); return;
		case InputKey::up: processKeyUp(keyboardInput.modControl); return;
		}
	}

	void TextEditWidget::onCharInputted(unsigned char character)
	{
		_textEditor->insertCursorChar(character);
		_currentColumnIndex = _textEditor->getCursorCharIndex();
	}

	void TextEditWidget::processKeyEnter(InputModControl modControl)
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

	void TextEditWidget::processKeyTab(InputModControl modControl)
	{
		static const std::wstring tabString = L"    ";
		_textEditor->insertAfterCursor(tabString);
		_textEditor->moveCursorToNextChar(tabString.length());
		_currentColumnIndex = _textEditor->getCursorCharIndex();
	}

	void TextEditWidget::processKeyBackspace(InputModControl modControl)
	{
		if (modControl.hasCtrlMod())
		{
			_textEditor->eraseWordBeforeCursor();
			_currentColumnIndex = _textEditor->getCursorCharIndex();
		}
		else if (!_textEditor->eraseCursorChar() && _currentRowIndex > 0)
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

	void TextEditWidget::processKeyDelete(InputModControl modControl)
	{
		if (modControl.hasCtrlMod())
		{
			if (!_textEditor->eraseWordAfterCursor() && _currentRowIndex + 1 < _lines.size())
			{
				_lines[_currentRowIndex]->setText(_lines[_currentRowIndex]->getText() + _lines[_currentRowIndex + 1]->getText());
				_lines.erase(_lines.begin() + _currentRowIndex + 1);
				_textEditor->eraseWordAfterCursor();
			}
		}
		else if (modControl.hasShiftMod())
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
		}
		// TODO: Copy paste is here, refactoring is required.
		else if (!_textEditor->moveCursorToNextChar() && _currentRowIndex < _lines.size() - 1)
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

	void TextEditWidget::processKeyRight(InputModControl modControl)
	{
		if (modControl.hasCtrlMod())
		{
			_textEditor->moveCursorToNextWord();
			_currentColumnIndex = _textEditor->getCursorCharIndex();
		}
		else if (!_textEditor->moveCursorToNextChar() && _currentRowIndex < _lines.size() - 1)
		{
			_currentRowIndex = std::clamp(_currentRowIndex + 1, (size_t)0, _lines.size() - 1);
			_textEditor->setTextRenderer(_lines[_currentRowIndex]);
			_textEditor->moveCursorToStart();
			setupTextEditor();
		}
		_currentColumnIndex = _textEditor->getCursorCharIndex();
	}

	void TextEditWidget::processKeyLeft(InputModControl modControl)
	{
		if (modControl.hasCtrlMod())
		{
			_textEditor->moveCursorToPrevWord();
			_currentColumnIndex = _textEditor->getCursorCharIndex();
		}
		else if (!_textEditor->moveCursorToPrevChar() && _currentRowIndex > 0)
		{
			// Use zu to explicitly specify the type of the literal (C++23 and above).
			_currentRowIndex = std::clamp(_currentRowIndex - 1, (size_t)0, _lines.size() - 1);
			_textEditor->setTextRenderer(_lines[_currentRowIndex]);
			_textEditor->moveCursorToEnd();
			setupTextEditor();
		}
		_currentColumnIndex = _textEditor->getCursorCharIndex();
	}

	void TextEditWidget::processKeyDown(InputModControl modControl)
	{
		if (_currentRowIndex < _lines.size() - 1)
		{
			_currentRowIndex = std::clamp(_currentRowIndex + 1, (size_t)0, _lines.size() - 1);
			_textEditor->setTextRenderer(_lines[_currentRowIndex]);
			_textEditor->moveCursorToNearest(_currentColumnIndex);
			setupTextEditor();
		}
	}

	void TextEditWidget::processKeyUp(InputModControl modControl)
	{
		if (_currentRowIndex > 0)
		{
			_currentRowIndex = std::clamp(_currentRowIndex - 1, (size_t)0, _lines.size() - 1);
			_textEditor->setTextRenderer(_lines[_currentRowIndex]);
			_textEditor->moveCursorToNearest(_currentColumnIndex);
			setupTextEditor();
		}
	}

}
