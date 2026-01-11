#include <rendell_ui/Widgets/private/TextEditor.h>

#include <rendell_text/IGlyphAtlasCache.h>
#include <rendell_text/ITextLayout.h>
#include <rendell_text/factory.h>
#include <rendell_ui/Widgets/private/TextEditorWord.h>

#include <rendell_ui/ITextModel.h>

#include <glm/glm.hpp>

namespace rendell_ui {
TextEditor::TextEditor(std::shared_ptr<ITextModel> textModel,
                       std::shared_ptr<rendell_text::IGlyphAtlasCache> glyphAtlasCache)
    : _textModel(textModel)
    , _glyphAtlasCache(glyphAtlasCache) {
    assert(_textModel);
    assert(_glyphAtlasCache);
    const auto textLayout = createTextLayout(U" ");
    _textModel->insertLine(0);
    _wordTypes = {
        makeDigitTextEditorWord(),
        makeVerbalTextEditorWord(),
        makeTextEditorWord(L" \t"),
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

const rendell_text::String &TextEditor::getText() const {
    assert(_textModel);
    return _textModel->getText();
}

void TextEditor::setText(rendell_text::String text) {
    assert(_textModel);
    _textModel->setText(std::move(text));
}

void TextEditor::setWordTypes(const std::vector<TextEditorWordSharedPtr> &value) {
    _wordTypes = value;
}

static bool isSpaceCharPredicate(wchar_t currentChar) {
    return currentChar == L' ';
}

size_t TextEditor::getCaretOffsetInString() const {
    assert(_textModel);
    return _textModel->getLineAdvance(_caret.y)[_caret.x];
}

uint32_t TextEditor::getCursorVerticalOffset() const {
    assert(_textModel);
    uint32_t result = 0;
    for (size_t i = 0; i < _caret.y; i++) {
        result += _textModel->getLineHeight(i);
    }
    return result - _textModel->getLineDescender(_caret.y);
}

uint32_t TextEditor::getCursorHorizontalOffset() const {
    assert(_textModel);

    if (_caret.x == 0) {
        return 0;
    }

    return _textModel->getOffsetInLine(_caret.y, _caret.x);
}

uint32_t TextEditor::getCursorHeight() const {
    assert(_textModel);
    return _textModel->getLineHeight(_caret.y);
}

const std::vector<TextEditorWordSharedPtr> &TextEditor::getWordTypes() const {
    return _wordTypes;
}

bool TextEditor::moveCursorToPrevChar(size_t count) {
    assert(_textModel);

    if (count == 0) {
        return false;
    }

    size_t caretX = _caret.x;
    size_t caretY = _caret.y;
    size_t remainingStepCount = count;
    while (remainingStepCount > 0) {
        if (remainingStepCount > caretX) {
            remainingStepCount -= caretX;
            if (caretY > 0) {
                remainingStepCount--;
                caretY--;
                caretX = _textModel->getLineLength(caretY);
            } else {
                caretX = 0;
                break;
            }
        } else {
            caretX -= remainingStepCount;
            remainingStepCount = 0;
            break;
        }
    }
    setCaret(caretX, caretY, true);
    return true;
}

bool TextEditor::moveCursorToNextChar(size_t count) {
    assert(_textModel);

    if (count == 0) {
        return false;
    }

    size_t caretX = _caret.x;
    size_t caretY = _caret.y;
    size_t remainingStepCount = count;
    while (remainingStepCount > 0) {
        if (const size_t delta = _textModel->getLineLength(caretY) - caretX;
            remainingStepCount > delta) {
            remainingStepCount -= delta;
            if (caretY < _textModel->getLineCount() - 1) {
                remainingStepCount--;
                caretY++;
                caretX = 0;
            } else {
                caretX = _textModel->getLineLength(caretY);
                break;
            }
        } else {
            caretX += remainingStepCount;
            remainingStepCount = 0;
            break;
        }
    }
    return setCaret(caretX, caretY, true);
}

bool TextEditor::moveCursorToPrevLine(size_t count) {
    assert(_textModel);

    if (count == 0) {
        return false;
    }

    const size_t caretY = count <= _caret.y ? _caret.y - count : 0;
    const size_t caretX = std::min(_caret.xCorrector, _textModel->getLineLength(caretY));
    return setCaret(caretX, caretY);
}

bool TextEditor::moveCursorToNextLine(size_t count) {
    assert(_textModel);

    if (count == 0) {
        return false;
    }

    const size_t caretY = std::min(_caret.y + count, _textModel->getLineCount() - 1);
    const size_t caretX = std::min(_caret.xCorrector, _textModel->getLineLength(caretY));
    return setCaret(caretX, caretY);
}

bool TextEditor::moveCursorToPrevWord() {
    return moveCursorToPrevChar(getPrevWordLength());
}

bool TextEditor::moveCursorToNextWord() {
    return moveCursorToNextChar(getNextWordLength());
}

bool TextEditor::moveCursorToStart() {
    setCaret(0, _caret.y, true);
    return true;
}

bool TextEditor::moveCursorToEnd() {
    assert(_textModel);
    setCaret(_textModel->getLineLength(_caret.y), _caret.y, true);
    return true;
}

void TextEditor::setupCursorByOffset(double x, double y) {
    const size_t caretY = getCaretYByOffset(y);
    const size_t caretX = getCaretXByOffset(caretY, x);
    setCaret(caretX, caretY, true);
}

bool TextEditor::eraseBeforeCursor(size_t count) {
    assert(_textModel);

    if (count == 0 || _caret.x == 0 && _caret.y == 0) {
        return false;
    }

    _textModel->eraseTextForward(_caret.y, _caret.x, count);
    return true;
}

bool TextEditor::eraseAfterCursor(size_t count) {
    assert(_textModel);

    if (count == 0) {
        return false;
    }

    _textModel->eraseTextBack(_caret.y, _caret.x, count);
    _caret.xCorrector = _caret.x;
}

bool TextEditor::eraseWordBeforeCursor() {
    return eraseBeforeCursor(getPrevWordLength());
}

bool TextEditor::eraseWordAfterCursor() {
    return eraseAfterCursor(getNextWordLength());
}

bool TextEditor::eraseLineUnderCursor() {
    assert(_textModel);

    if (_textModel->getLineCount() == 1) {
        if (_textModel->getLineLength(0) > 0) {
            _textModel->eraseLine(0);
            setCaret(0, 0);
            return true;
        }
        return false;
    }

    _textModel->eraseLine(_caret.y);
    return true;
}

bool TextEditor::insertAfterCursor(const rendell_text::String &text) {
    assert(_textModel);

    if (text.empty()) {
        return false;
    }

    _textModel->insertText(_caret.y, _caret.y, text);
    return true;
}

bool TextEditor::moveLineUnderCursorDown() {
    assert(_textModel);
    if (_caret.y + 1 < _textModel->getLineCount()) {
        _textModel->swipeLines(_caret.y, _caret.y + 1);
        setCaret(_caret.x, _caret.y + 1, true);
        return true;
    }
    return false;
}

bool TextEditor::moveLineUnderCursorUp() {
    assert(_textModel);
    if (_caret.y > 0) {
        _textModel->swipeLines(_caret.y, _caret.y - 1);
        setCaret(_caret.x, _caret.y - 1, true);
        return true;
    }
    return false;
}

std::shared_ptr<rendell_text::ITextLayout>
TextEditor::createTextLayout(rendell_text::String &&text) {
    assert(_glyphAtlasCache);
    auto result = rendell_text::createTextLayout(_glyphAtlasCache);
    result->setText(std::move(text));
    return result;
}

bool TextEditor::setCaret(size_t x, size_t y, bool setXCorrector) {
    if (setXCorrector) {
        _caret.xCorrector = x;
    }

    if (_caret.x != x || _caret.y != y) {
        _caret.x = x;
        _caret.y = y;
        cursorChanged.emit(getCursorHorizontalOffset(), getCursorVerticalOffset(),
                           getCursorHeight());
        return true;
    }
    return false;
}

bool TextEditor::isSameWord(const TextEditorWordSharedPtr &word, wchar_t character) const {
    if (word) {
        if (word->isWordCharacter(character)) {
            return true;
        }
    } else if (findWord(character) == nullptr) {
        return true;
    }
    return false;
}

size_t TextEditor::getPrevWordLength() const {
    assert(_textModel);

    size_t result = 0;
    size_t caretX = _caret.x;
    size_t caretY = _caret.y;
    if (caretX == 0 && caretY > 0) {
        caretY--;
        caretX = _textModel->getLineLength(caretY);
        result++;
    }

    if (const rendell_text::String &text = _textModel->getLineText(caretY); text.length() > 0) {
        size_t i = caretX;
        if (i > 0) {
            const TextEditorWordSharedPtr &word = findWord(text[i - 1]);
            do {
                result++;
                i--;
            } while (i > 0 && isSameWord(word, text[i - 1]));
        }
    }
    return result;
}

size_t TextEditor::getNextWordLength() const {
    size_t result = 0;
    size_t caretX = _caret.x;
    size_t caretY = _caret.y;
    if (caretX == _textModel->getLineLength(caretY) && caretY < _textModel->getLineCount() - 1) {
        caretY++;
        caretX = 0;
        result++;
    }

    if (const rendell_text::String &text = _textModel->getLineText(caretY); text.size() > 0) {
        size_t i = caretX;
        const TextEditorWordSharedPtr &word = findWord(text[i]);
        do {
            result++;
            i++;
        } while (i < text.length() && isSameWord(word, text[i]));
    }
    return result;
}

size_t TextEditor::getCaretYByOffset(double offset) const {
    assert(_textModel);
    assert(_textModel->getLineCount() > 0);
    return _textModel->getLineIndexByOffset(offset);
}

size_t TextEditor::getCaretXByOffset(size_t caretY, double offset) const {
    assert(_textModel);
    assert(caretY < _textModel->getLineCount());
    return _textModel->getCharIndexByOffset(caretY, offset);
}

TextEditorWordSharedPtr TextEditor::findWord(wchar_t character) const {
    for (const TextEditorWordSharedPtr &word : _wordTypes) {
        if (word->isWordCharacter(character)) {
            return word;
        }
    }
    return nullptr;
}

} // namespace rendell_ui