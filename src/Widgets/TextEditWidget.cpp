#include "TextEditWidget.h"
#include "../Viewport.h"
#include "../String/StringExtension.h"
#include "private/Cursor.h"
#include <glm/gtc/matrix_transform.hpp>

namespace rendell_ui
{
	static rendell_text::TextRendererSharedPtr createTextRenderer(const rendell_text::TextLayoutSharedPtr& textLayout)
	{
		rendell_text::TextRendererSharedPtr result = rendell_text::makeTextRenderer();
		result->setTextLayout(textLayout);
		result->setBackgroundColor(glm::vec4(31.0f / 255.0, 31.0f / 255.0, 31.0f / 255.0, 1.0f));
		result->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		return result;
	}

	TextEditWidget::TextEditWidget() : Widget()
	{
		_textLayoutClearedConnectionId = _textEditor.textLayoutCleared.connect(this, &TextEditWidget::onTextLayoutCleared);
		_textLayoutRemovedConnectionId = _textEditor.textLayoutRemoved.connect(this, &TextEditWidget::onTextLayoutRemoved);
		_textLayoutAddedConnectionId = _textEditor.textLayoutAdded.connect(this, &TextEditWidget::onTextLayoutAdded);
		_caretChangedConnectionId = _textEditor.cursorChanged.connect(this, &TextEditWidget::onCaretChanged);
	}

	TextEditWidget::~TextEditWidget()
	{
		_textEditor.textLayoutCleared.disconnect(_textLayoutClearedConnectionId);
		_textEditor.textLayoutRemoved.disconnect(_textLayoutRemovedConnectionId);
		_textEditor.textLayoutAdded.disconnect(_textLayoutAddedConnectionId);
		_textEditor.cursorChanged.disconnect(_caretChangedConnectionId);
	}

	void TextEditWidget::draw() const
	{
		const glm::mat4& projectMat = Viewport::getCurrent()->getProjectMat();
		const glm::mat4& viewMat = Viewport::getCurrent()->getViewMat();
		const glm::mat4& transformMat = _transform.getMatrix();
		float height = _size.y * 0.5f - _fontSize.y;

		for (const rendell_text::TextRendererSharedPtr& textRenderer : _textRenderers)
		{
			const glm::mat4 worldMat = glm::translate(transformMat, glm::vec3(-_size.x * 0.5f, height, 0.0f));
			height -= textRenderer->getTextLayout()->getFontHeight();

			textRenderer->setMatrix(projectMat * viewMat * worldMat);
			textRenderer->draw();
		}
	}

	const std::wstring& TextEditWidget::getText() const
	{
		return _textEditor.getText();
	}

	void TextEditWidget::setText(const std::wstring& value)
	{
		_textEditor.setText(value);
	}

	void TextEditWidget::setFontSize(glm::ivec2 value)
	{
		_textEditor.setFontSize(value);
	}

	void TextEditWidget::onSelfWeakPtrChanged()
	{
		_cursor = createCursor(_selfWeakPtr);
		_cursor->setVisible(false);
		_cursor->setAnchor(Anchor::leftTop);
		_cursor->setHeight(_textEditor.getCursorHeight());
	}

	void TextEditWidget::onTextLayoutCleared()
	{
		_textRenderers.clear();
	}

	void TextEditWidget::onTextLayoutRemoved(size_t index)
	{
		_textRenderers.erase(_textRenderers.begin() + index);
	}

	void TextEditWidget::onTextLayoutAdded(size_t index, const rendell_text::TextLayoutSharedPtr& textLayout)
	{
		rendell_text::TextRendererSharedPtr& textRenderer = createTextRenderer(textLayout);
		_textRenderers.insert(_textRenderers.begin() + index, textRenderer);
	}

	void TextEditWidget::onCaretChanged(uint32_t x, uint32_t y, uint32_t height)
	{
		_cursor->setOffset({ x, -static_cast<float>(y) });
		_cursor->setHeight(height);
		_cursor->resetBlinkTimer();
		_cursor->updateRecursively();
	}

	void TextEditWidget::onFocused()
	{
		_cursor->setVisible(true);
	}

	void TextEditWidget::onUnfocused()
	{
		_cursor->setVisible(false);
	}

	void TextEditWidget::onMouseDown(glm::dvec2 cursorPosition)
	{
		cursorPosition = static_cast<glm::dvec2>(_size * 0.5f) -
			cursorPosition - static_cast<glm::dvec2>(_transform.getPosition());
		_textEditor.setupCursorByOffset(cursorPosition.x, cursorPosition.y);
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
		const std::wstring string(1, static_cast<wchar_t>(character));
		_textEditor.insertAfterCursor(string);
	}

	void TextEditWidget::processKeyEnter(InputModControl modControl)
	{
		_textEditor.insertAfterCursor(L"\n");
	}

	void TextEditWidget::processKeyTab(InputModControl modControl)
	{
		static const std::wstring tabString = L"    ";
		_textEditor.insertAfterCursor(tabString);
	}

	void TextEditWidget::processKeyBackspace(InputModControl modControl)
	{
		if (modControl.hasCtrlMod())
		{
			_textEditor.eraseWordBeforeCursor();
		}
		else
		{
			_textEditor.eraseBeforeCursor();
		}
	}

	void TextEditWidget::processKeyDelete(InputModControl modControl)
	{
		if (modControl.hasCtrlMod())
		{
			_textEditor.eraseWordAfterCursor();
		}
		else if (modControl.hasShiftMod())
		{
			_textEditor.eraseLineUnderCursor();
		}
		else
		{
			_textEditor.eraseAfterCursor();
		}
	}

	void TextEditWidget::processKeyRight(InputModControl modControl)
	{
		if (modControl.hasCtrlMod())
		{
			_textEditor.moveCursorToNextWord();
		}
		else
		{
			_textEditor.moveCursorToNextChar();
		}
	}

	void TextEditWidget::processKeyLeft(InputModControl modControl)
	{
		if (modControl.hasCtrlMod())
		{
			_textEditor.moveCursorToPrevWord();
		}
		else
		{
			_textEditor.moveCursorToPrevChar();
		}
	}

	void TextEditWidget::processKeyDown(InputModControl modControl)
	{
		_textEditor.moveCursorToNextLine();
	}

	void TextEditWidget::processKeyUp(InputModControl modControl)
	{
		_textEditor.moveCursorToPrevLine();
	}

}
