#include <rendell_ui/Widgets/TextEditWidget.h>
#include <rendell_ui/Widgets/private/Cursor.h>
#include <rendell_ui/Viewport.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../String/StringExtension.h"

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
		_textLayoutSwappedConnectionId = _textEditor.textLayoutSwapped.connect(this, &TextEditWidget::onTextLayoutSwapped);
		_caretChangedConnectionId = _textEditor.cursorChanged.connect(this, &TextEditWidget::onCaretChanged);
	}

	TextEditWidget::~TextEditWidget()
	{
		_textEditor.textLayoutCleared.disconnect(_textLayoutClearedConnectionId);
		_textEditor.textLayoutRemoved.disconnect(_textLayoutRemovedConnectionId);
		_textEditor.textLayoutAdded.disconnect(_textLayoutAddedConnectionId);
		_textEditor.textLayoutSwapped.disconnect(_textLayoutSwappedConnectionId);
		_textEditor.cursorChanged.disconnect(_caretChangedConnectionId);
	}

	void TextEditWidget::draw() const
	{
		const glm::mat4& projectMat = Viewport::getCurrent()->getProjectMat();
		const glm::mat4& viewMat = Viewport::getCurrent()->getViewMat();
		const glm::mat4& transformMat = _transform.getMatrix();
		
		float currentOffset = _size.y * 0.5f + _startRenderingOffset;
		for (size_t i = _startRenderingIndex; i < _textRenderers.size(); i++)
		{
			const rendell_text::TextRendererSharedPtr& textRenderer = _textRenderers[i];
			currentOffset -= textRenderer->getTextLayout()->getFontHeight();
			if (currentOffset < -_size.y)
			{
				break;
			}
			const glm::mat4 worldMat = glm::translate(transformMat, glm::vec3(-_size.x * 0.5f, currentOffset, 0.0f));
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
		_cursor->setHeight(static_cast<float>(_textEditor.getCursorHeight()));
	}

	void TextEditWidget::onTextLayoutCleared()
	{
		_textRenderers.clear();
		_textHeight = 0;
	}

	void TextEditWidget::onTextLayoutRemoved(size_t index)
	{
		auto it = _textRenderers.begin() + index;
		_textHeight -= it->operator->()->getTextLayout()->getFontHeight();
		_textRenderers.erase(it);
	}

	void TextEditWidget::onTextLayoutAdded(size_t index, const rendell_text::TextLayoutSharedPtr& textLayout)
	{
		const rendell_text::TextRendererSharedPtr& textRenderer = createTextRenderer(textLayout);
		_textRenderers.insert(_textRenderers.begin() + index, textRenderer);
		_textHeight += textLayout->getFontHeight();
	}

	void TextEditWidget::onTextLayoutSwapped(size_t firstIndex, size_t secondIndex)
	{
		std::swap(_textRenderers[firstIndex], _textRenderers[secondIndex]);
	}

	void TextEditWidget::onCaretChanged(uint32_t x, uint32_t y, uint32_t height)
	{
		_cursor->setHeight(static_cast<float>(height));
		if (!updateScrollOffset(_scrollOffset))
		{
			_cursor->setOffset({ static_cast<float>(x), -static_cast<float>(y) + _scrollOffset });
			_cursor->resetBlinkTimer();
			_cursor->updateRecursively();
		}
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
		const glm::dvec2 localPosition = cursorPosition - static_cast<glm::dvec2>(_transform.getPosition());
		const double x = localPosition.x + _size.x * 0.5;
		const double y = -(localPosition.y - _size.y * 0.5) + _scrollOffset;
		_textEditor.setupCursorByOffset(x, y);
	}

	void TextEditWidget::onMouseScrolled(glm::dvec2 scroll)
	{
		const float scrollY = static_cast<float>(scroll.y) * 50.0f;
		updateScrollOffset(_scrollOffset - scrollY);
	}

	void TextEditWidget::onKeyInputted(const KeyboardInput& keyboardInput)
	{
		if (keyboardInput.action != InputAction::pressed && keyboardInput.action != InputAction::repeat)
		{
			return;
		}

		switch (keyboardInput.key)
		{
		case InputKey::X: processKeyX(keyboardInput.modControl); break;
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
		if (modControl.hasAltMod())
		{
			_textEditor.moveLineUnderCursorDown();
			return;
		}
		_textEditor.moveCursorToNextLine();
	}

	void TextEditWidget::processKeyUp(InputModControl modControl)
	{
		if (modControl.hasAltMod())
		{
			_textEditor.moveLineUnderCursorUp();
			return;
		}
		_textEditor.moveCursorToPrevLine();
	}

	void TextEditWidget::processKeyX(InputModControl modControl)
	{
		if (modControl.hasCtrlMod())
		{
			_textEditor.eraseLineUnderCursor();
		}
	}

	bool TextEditWidget::updateScrollOffset(float value)
	{
		const float maxScrollOffset = _textHeight > _size.y ? static_cast<float>(_textHeight) - _size.y: 0.0f;
		const float newScrollOffset = std::clamp(value, 0.0f, maxScrollOffset);
		if (_scrollOffset != newScrollOffset)
		{
			_scrollOffset = newScrollOffset;
			const float cursorOffsetX = static_cast<float>(_textEditor.getCursorHorizontalOffset());
			const float cursorOffsetY = -static_cast<float>(_textEditor.getCursorVerticalOffset()) + _scrollOffset;
			_cursor->setOffset({ cursorOffsetX, cursorOffsetY });
			_cursor->updateRecursively();
			optimizeRendering();
			return true;
		}
		return false;
	}

	void TextEditWidget::optimizeRendering()
	{
		const float baseOffset = _size.y * 0.5f;
		float currentOffset = baseOffset + _scrollOffset;
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
