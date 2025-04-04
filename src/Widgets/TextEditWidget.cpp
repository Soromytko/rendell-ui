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
		setName("TextEditWidget");
		_textLayoutClearedConnectionId = _textEditor.textLayoutCleared.connect(this, &TextEditWidget::onTextLayoutCleared);
		_textLayoutRemovedConnectionId = _textEditor.textLayoutRemoved.connect(this, &TextEditWidget::onTextLayoutRemoved);
		_textLayoutAddedConnectionId = _textEditor.textLayoutAdded.connect(this, &TextEditWidget::onTextLayoutAdded);
		_textLayoutSwappedConnectionId = _textEditor.textLayoutSwapped.connect(this, &TextEditWidget::onTextLayoutSwapped);
		_caretChangedConnectionId = _textEditor.cursorChanged.connect(this, &TextEditWidget::onCaretChanged);
		_textChangedConnectionId = _textEditor.textChanged.connect(textChanged);

		_scrollbarWidget = createUnregisteredScrollbarWidget();
		_scrollbarWidget->setAnchor(Anchor::rightStretch);
	}

	TextEditWidget::~TextEditWidget()
	{
		_textEditor.textLayoutCleared.disconnect(_textLayoutClearedConnectionId);
		_textEditor.textLayoutRemoved.disconnect(_textLayoutRemovedConnectionId);
		_textEditor.textLayoutAdded.disconnect(_textLayoutAddedConnectionId);
		_textEditor.textLayoutSwapped.disconnect(_textLayoutSwappedConnectionId);
		_textEditor.cursorChanged.disconnect(_caretChangedConnectionId);
		_textEditor.cursorChanged.disconnect(_textChangedConnectionId);
	}

	void TextEditWidget::draw() const
	{
		const glm::mat4& projectMat = Viewport::getCurrent()->getProjectMat();
		const glm::mat4& viewMat = Viewport::getCurrent()->getViewMat();
		const glm::mat4& transformMat = _transform.getMatrix();

		_textDrawer.draw(projectMat * viewMat, transformMat);

		if (_cursor->getVisible())
		{
			_cursor->draw();
		}

		if (_scrollbarWidget->getVisible())
		{
			_scrollbarWidget->draw();
		}
	}

	float TextEditWidget::getScrollProgress() const
	{
		return _textDrawer.getScrollProgress();
	}

	float TextEditWidget::getScrollRatio() const
	{
		return _size.y / static_cast<float>(_textDrawer.getTextHeight());
	}

	bool TextEditWidget::setScrollProgress(float value)
	{
		if (_textDrawer.setScrollProgress(value))
		{
			updateCursorPosition();
			scrollProgressChanged.emit(_textDrawer.getScrollProgress());
			return true;
		}
		return false;
	}

	const std::wstring& TextEditWidget::getText() const
	{
		return _textEditor.getText();
	}

	size_t TextEditWidget::getLineCount() const
	{
		return _textDrawer.getLines().size();
	}

	bool TextEditWidget::getScrollEnabled() const
	{
		return _scrollEnabled;
	}

	void TextEditWidget::setText(const std::wstring& value)
	{
		_textEditor.setText(value);
	}

	void TextEditWidget::setFontSize(glm::ivec2 value)
	{
		_textEditor.setFontSize(value);
	}

	void TextEditWidget::setScrollEnabled(bool value)
	{
		_scrollEnabled = value;
		_scrollbarWidget->setVisible(_scrollEnabled);
	}

	void TextEditWidget::onSelfWeakPtrChanged()
	{
		_cursor = createUnregisteredCursor(_selfWeakPtr);
		_cursor->setVisible(false);
		_cursor->setAnchor(Anchor::leftTop);
		_cursor->setHeight(static_cast<float>(_textEditor.getCursorHeight()));

		_scrollbarWidget->setParent(_selfWeakPtr);
		IScrollableWidgetSharedPtr scrollable = std::dynamic_pointer_cast<IScrollableWidget>(_selfWeakPtr.lock());
		_scrollbarWidget->setScrollable(scrollable);
	}

	void TextEditWidget::onDragged(glm::dvec2 startPoint, glm::dvec2 endPoint)
	{
		if (_scrollEnabled && _scrollbarWidget->intersect(startPoint))
		{
			_scrollbarWidget->onDragged(startPoint, endPoint);
			return;
		}
	}

	void TextEditWidget::onSizeChanged()
	{
		_textDrawer.setSize(_size);
	}

	void TextEditWidget::onTextLayoutCleared()
	{
		_textDrawer.clear();
		_scrollbarWidget->updateProgress();
	}

	void TextEditWidget::onTextLayoutRemoved(size_t index)
	{
		_textDrawer.removeLine(index);
		_scrollbarWidget->updateProgress();
	}

	void TextEditWidget::onTextLayoutAdded(size_t index, const rendell_text::TextLayoutSharedPtr& textLayout)
	{
		const rendell_text::TextRendererSharedPtr& textRenderer = createTextRenderer(textLayout);
		_textDrawer.addLine(index, textRenderer);
		_scrollbarWidget->updateProgress();
	}

	void TextEditWidget::onTextLayoutSwapped(size_t firstIndex, size_t secondIndex)
	{
		_textDrawer.swapLines(firstIndex, secondIndex);
	}

	void TextEditWidget::onCaretChanged(uint32_t x, uint32_t y, uint32_t height)
	{
		_cursor->setHeight(static_cast<float>(height));
		_cursor->resetBlinkTimer();
		updateCursorPosition();
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
		const double y = -(localPosition.y - _size.y * 0.5) + _textDrawer.getScroll();
		_textEditor.setupCursorByOffset(x, y);
	}

	void TextEditWidget::onMouseScrolled(glm::dvec2 scroll)
	{
		const float scrollProgresssDelta = -static_cast<float>(scroll.y) * getScrollRatio() * 0.1f;// *50.0f;
		if (setScrollProgress(getScrollProgress() + scrollProgresssDelta))
		{
			_scrollbarWidget->updateProgress();
		}
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

	void TextEditWidget::updateCursorPosition()
	{
		const float cursorOffsetX = static_cast<float>(_textEditor.getCursorHorizontalOffset());
		const float cursorOffsetY = -static_cast<float>(_textEditor.getCursorVerticalOffset()) + _textDrawer.getScroll();
		_cursor->setOffset({ cursorOffsetX, cursorOffsetY });
	}

}
