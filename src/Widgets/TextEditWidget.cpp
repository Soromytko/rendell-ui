#include <glm/gtc/matrix_transform.hpp>
#include <rendell_ui/Viewport.h>
#include <rendell_ui/Widgets/TextEditWidget.h>
#include <rendell_ui/Widgets/private/Cursor.h>

#include <rendell_text/factory.h>

#include <FontStorage.h>
#include <TextModel.h>
#include <rendell_ui/ITextModel.h>

namespace rendell_ui {
TextEditWidget::TextEditWidget()
    : Widget()
    , _glyphAtlasCache(FontStorage::getInstance()->getDefaultGlyphAtlasCache())
    , _textModel(std::make_shared<TextModel>(_glyphAtlasCache))
    , _textEditor(_textModel, _glyphAtlasCache)
    , _textDrawer(_textModel) {
    setName("TextEditWidget");
    _textLayoutClearedConnectionId =
        _textEditor.textLayoutCleared.connect(this, &TextEditWidget::onTextLayoutCleared);
    _textLayoutRemovedConnectionId =
        _textEditor.textLayoutRemoved.connect(this, &TextEditWidget::onTextLayoutRemoved);
    _textLayoutAddedConnectionId =
        _textEditor.textLayoutAdded.connect(this, &TextEditWidget::onTextLayoutAdded);
    _textLayoutSwappedConnectionId =
        _textEditor.textLayoutSwapped.connect(this, &TextEditWidget::onTextLayoutSwapped);
    _caretChangedConnectionId =
        _textEditor.cursorChanged.connect(this, &TextEditWidget::onCaretChanged);
    _textChangedConnectionId = _textEditor.textChanged.connect(textChanged);

    _scrollbarWidget = createUnregisteredScrollbarWidget();
    _scrollbarWidget->setAnchor(Anchor::rightStretch);
}

TextEditWidget::~TextEditWidget() {
    _textEditor.textLayoutCleared.disconnect(_textLayoutClearedConnectionId);
    _textEditor.textLayoutRemoved.disconnect(_textLayoutRemovedConnectionId);
    _textEditor.textLayoutAdded.disconnect(_textLayoutAddedConnectionId);
    _textEditor.textLayoutSwapped.disconnect(_textLayoutSwappedConnectionId);
    _textEditor.cursorChanged.disconnect(_caretChangedConnectionId);
    _textEditor.textChanged.disconnect(_textChangedConnectionId);
}

void TextEditWidget::draw() {
    const glm::mat4 &projectMat = Viewport::getCurrent()->getProjectMat();
    const glm::mat4 &viewMat = Viewport::getCurrent()->getViewMat();
    const glm::mat4 &transformMat = glm::translate(
        _transform.getMatrix(), glm::vec3(-_scrollbarWidget->getSize().x * 0.5f, 0.0f, 0.0f));

    const auto windowSize = Viewport::getCurrent()->getSize();

    _textDrawer.draw(projectMat * viewMat, transformMat);

    const glm::ivec2 textDrawerSize = static_cast<glm::ivec2>(_textDrawer.getSize());
    Viewport::getCurrent()->setScissors(static_cast<int>(transformMat[3].x),
                                        static_cast<int>(transformMat[3].y), textDrawerSize.x,
                                        textDrawerSize.y);
    if (_cursor->getVisible()) {
        _cursor->draw();
    }

    if (_scrollbarWidget->getVisible()) {
        _scrollbarWidget->draw();
    }
}

double TextEditWidget::getScrollProgress() const {
    return _textDrawer.getScrollProgress();
}

double TextEditWidget::getScrollRatio() const {
    return _size.y / static_cast<float>(_textDrawer.getTextHeight());
}

bool TextEditWidget::setScrollProgress(double value) {
    if (_textDrawer.setScrollProgress(value)) {
        updateCursorPosition();
        scrollProgressChanged.emit(_textDrawer.getScrollProgress());
        return true;
    }
    return false;
}

void TextEditWidget::onProcessMouseScrolled(glm::dvec2 scroll) {
    onMouseScrolled(scroll);
}

const rendell_text::String &TextEditWidget::getText() const {
    return _textEditor.getText();
}

bool TextEditWidget::getScrollEnabled() const {
    return _scrollEnabled;
}

double TextEditWidget::getScrollSensitivity() const {
    return _scrollSensitivity;
}

void TextEditWidget::setText(rendell_text::String value) {
    _textEditor.setText(std::move(value));
}

void TextEditWidget::setScrollEnabled(bool value) {
    _scrollEnabled = value;
    _scrollbarWidget->setVisible(_scrollEnabled);
}

void TextEditWidget::setScrollSensitivity(double value) {
    _scrollSensitivity = value;
}

void TextEditWidget::onSelfWeakPtrChanged() {
    _cursor = createUnregisteredCursor(_selfWeakPtr);
    _cursor->setVisible(false);
    _cursor->setAnchor(Anchor::leftTop);
    _cursor->setHeight(static_cast<float>(_textEditor.getCursorHeight()));

    _scrollbarWidget->setParent(_selfWeakPtr);
    IScrollableWidgetSharedPtr scrollable =
        std::dynamic_pointer_cast<IScrollableWidget>(_selfWeakPtr.lock());
    _scrollbarWidget->setScrollable(scrollable);
}

void TextEditWidget::onSizeChanged() {
    _textDrawer.setSize(_size - glm::vec2(_scrollbarWidget->getSize().x, 0.0f));
}

void TextEditWidget::onTextLayoutCleared() {
    _textDrawer.clear();
    _scrollbarWidget->updateProgress();
}

void TextEditWidget::onTextLayoutRemoved(size_t index) {
    _textDrawer.removeLine(index);
    _scrollbarWidget->updateProgress();
}

void TextEditWidget::onTextLayoutSwapped(size_t firstIndex, size_t secondIndex) {
    _textDrawer.swapLines(firstIndex, secondIndex);
}

void TextEditWidget::onCaretChanged(uint32_t x, uint32_t y, uint32_t height) {
    _cursor->setHeight(static_cast<float>(height));
    _cursor->resetBlinkTimer();
    updateCursorPosition();
}

void TextEditWidget::onFocused() {
    _cursor->setVisible(true);
}

void TextEditWidget::onUnfocused() {
    _cursor->setVisible(false);
}

void TextEditWidget::onMouseDown(glm::dvec2 cursorPosition) {
    const glm::dvec2 localPosition =
        cursorPosition - static_cast<glm::dvec2>(_transform.getPosition());
    const double x = localPosition.x + _size.x * 0.5;
    const double y = -(localPosition.y - _size.y * 0.5) + _textDrawer.getScroll();
    _textEditor.setupCursorByOffset(x, y);
}

void TextEditWidget::onMouseHovered(glm::dvec2 cursorPosition) {
    if (!_scrollbarWidget->getIsHidden() && _scrollbarWidget->intersect(cursorPosition)) {
        setWindowCursorType(WindowCursorType::arrow);
    } else {
        setWindowCursorType(WindowCursorType::ibeam);
    }
}

void TextEditWidget::onMouseEntered() {
    setWindowCursorType(WindowCursorType::ibeam);
}

void TextEditWidget::onMouseExited() {
    setWindowCursorType(WindowCursorType::arrow);
}

void TextEditWidget::onMouseScrolled(glm::dvec2 scroll) {
    const double newScrollProgresss = (_textDrawer.getScroll() - scroll.y * _scrollSensitivity) /
                                      (_textDrawer.getTextHeight() - _textDrawer.getSize().y);
    if (setScrollProgress(newScrollProgresss)) {
        _scrollbarWidget->updateProgress();
    }
}

void TextEditWidget::onKeyInputted(const KeyboardInput &keyboardInput) {
    if (keyboardInput.action != InputAction::pressed &&
        keyboardInput.action != InputAction::repeat) {
        return;
    }

    switch (keyboardInput.key) {
    case InputKey::X:
        processKeyX(keyboardInput.modControl);
        break;
    case InputKey::enter:
        processKeyEnter(keyboardInput.modControl);
        return;
    case InputKey::tab:
        processKeyTab(keyboardInput.modControl);
        return;
    case InputKey::backspace:
        processKeyBackspace(keyboardInput.modControl);
        return;
    case InputKey::del:
        processKeyDelete(keyboardInput.modControl);
        return;
    case InputKey::right:
        processKeyRight(keyboardInput.modControl);
        return;
    case InputKey::left:
        processKeyLeft(keyboardInput.modControl);
        return;
    case InputKey::down:
        processKeyDown(keyboardInput.modControl);
        return;
    case InputKey::up:
        processKeyUp(keyboardInput.modControl);
        return;
    }
}

void TextEditWidget::onCharInputted(unsigned char character) {
    const rendell_text::String string(1, static_cast<wchar_t>(character));
    _textEditor.insertAfterCursor(string);
}

void TextEditWidget::processKeyEnter(InputModControl modControl) {
    _textEditor.insertAfterCursor(U"\n");
}

void TextEditWidget::processKeyTab(InputModControl modControl) {
    static const rendell_text::String tabString = U"    ";
    _textEditor.insertAfterCursor(tabString);
}

void TextEditWidget::processKeyBackspace(InputModControl modControl) {
    if (modControl.hasCtrlMod()) {
        _textEditor.eraseWordBeforeCursor();
    } else {
        _textEditor.eraseBeforeCursor();
    }
}

void TextEditWidget::processKeyDelete(InputModControl modControl) {
    if (modControl.hasCtrlMod()) {
        _textEditor.eraseWordAfterCursor();
    } else if (modControl.hasShiftMod()) {
        _textEditor.eraseLineUnderCursor();
    } else {
        _textEditor.eraseAfterCursor();
    }
}

void TextEditWidget::processKeyRight(InputModControl modControl) {
    if (modControl.hasCtrlMod()) {
        _textEditor.moveCursorToNextWord();
    } else {
        _textEditor.moveCursorToNextChar();
    }
}

void TextEditWidget::processKeyLeft(InputModControl modControl) {
    if (modControl.hasCtrlMod()) {
        _textEditor.moveCursorToPrevWord();
    } else {
        _textEditor.moveCursorToPrevChar();
    }
}

void TextEditWidget::processKeyDown(InputModControl modControl) {
    if (modControl.hasAltMod()) {
        _textEditor.moveLineUnderCursorDown();
        return;
    }
    _textEditor.moveCursorToNextLine();
}

void TextEditWidget::processKeyUp(InputModControl modControl) {
    if (modControl.hasAltMod()) {
        _textEditor.moveLineUnderCursorUp();
        return;
    }
    _textEditor.moveCursorToPrevLine();
}

void TextEditWidget::processKeyX(InputModControl modControl) {
    if (modControl.hasCtrlMod()) {
        _textEditor.eraseLineUnderCursor();
    }
}

void TextEditWidget::updateCursorPosition() {
    const float cursorOffsetX = static_cast<float>(_textEditor.getCursorHorizontalOffset());
    const float cursorOffsetY = -static_cast<float>(_textEditor.getCursorVerticalOffset()) +
                                static_cast<float>(_textDrawer.getScroll());
    _cursor->setOffset({cursorOffsetX, cursorOffsetY});
}

} // namespace rendell_ui
