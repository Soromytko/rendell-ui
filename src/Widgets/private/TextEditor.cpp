#include "../../String/StringExtension.h"
#include <rendell_text/IGlyphAtlasCache.h>
#include <rendell_text/ITextLayout.h>
#include <rendell_text/factory.h>
#include <rendell_ui/Widgets/private/TextEditor.h>
#include <rendell_ui/Widgets/private/TextEditorWord.h>

#include <algorithm>
#include <glm/glm.hpp>

namespace rendell_ui {
TextEditor::TextEditor(std::shared_ptr<rendell_text::IGlyphAtlasCache> glyphAtlasCache)
    : _glyphAtlasCache(glyphAtlasCache) {
    assert(_glyphAtlasCache);
    const auto textLayout = createTextLayout(U" ");
    addTextLayout(0, textLayout);
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
    updateCachedTextIfNeeded();
    return _cachedText;
}

void TextEditor::setText(rendell_text::String &&text) {
    if (!_shouldCachedTextBeUpdated && _cachedText == text) {
        return;
    }

    std::vector<rendell_text::String> lines = StringExtension::split(text, U"\n");
    const size_t oldLineCount = lines.size();
    _textLayouts.reserve(lines.size());

    if (oldLineCount < _textLayouts.size()) {
        for (size_t i = 0; i < oldLineCount; i++) {
            rendell_text::String lineText = std::move(lines[i]);
            _textLayouts[i]->setText(lineText);
        }
        for (size_t i = oldLineCount; i < _textLayouts.size(); i++) {
            rendell_text::String lineText = std::move(lines[i]);
            auto textLayout = createTextLayout(std::move(lineText));
            _textLayouts[i] = textLayout;
        }
        return;
    }

    for (size_t i = 0; i < _textLayouts.size(); i++) {
        rendell_text::String lineText = std::move(lines[i]);
        _textLayouts[i]->setText(std::move(lineText));
    }

    _cachedText = std::move(text);
    _shouldCachedTextBeUpdated = false;
    textChanged.emit();
}

void TextEditor::setWordTypes(const std::vector<TextEditorWordSharedPtr> &value) {
    _wordTypes = value;
}

static bool isSpaceCharPredicate(wchar_t currentChar) {
    return currentChar == L' ';
}

size_t TextEditor::getCaretOffsetInString() const {
    return _textLayouts[_caret.y]->getTextAdvance()[_caret.x];
}

uint32_t TextEditor::getCursorVerticalOffset() const {
    uint32_t result = 0;
    for (size_t i = 0; i < _caret.y; i++) {
        result += _textLayouts[i]->getGlyphAtlasCache()->getLineHeight();
    }
    return result - _textLayouts[_caret.y]->getDescender();
}

uint32_t TextEditor::getCursorHorizontalOffset() const {
    if (_caret.x == 0) {
        return 0;
    }

    const auto &textLayout = _textLayouts[_caret.y];
    if (const size_t textLength = textLayout->getTextLength(); _caret.x <= textLength) {
        return textLayout->getTextAdvance()[_caret.x - 1];
    }
    return 0;
}

uint32_t TextEditor::getCursorHeight() const {
    return _textLayouts[_caret.y]->getGlyphAtlasCache()->getLineHeight();
}

const std::vector<TextEditorWordSharedPtr> &TextEditor::getWordTypes() const {
    return _wordTypes;
}

bool TextEditor::moveCursorToPrevChar(size_t count) {
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
                caretX = _textLayouts[caretY]->getTextLength();
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
    if (count == 0) {
        return false;
    }

    size_t caretX = _caret.x;
    size_t caretY = _caret.y;
    size_t remainingStepCount = count;
    while (remainingStepCount > 0) {
        if (const size_t delta = _textLayouts[caretY]->getTextLength() - caretX;
            remainingStepCount > delta) {
            remainingStepCount -= delta;
            if (caretY < _textLayouts.size() - 1) {
                remainingStepCount--;
                caretY++;
                caretX = 0;
            } else {
                caretX = _textLayouts[caretY]->getTextLength();
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
    if (count == 0) {
        return false;
    }

    const size_t caretY = count <= _caret.y ? _caret.y - count : 0;
    const size_t caretX = std::min(_caret.xCorrector, _textLayouts[caretY]->getTextLength());
    return setCaret(caretX, caretY);
    ;
}

bool TextEditor::moveCursorToNextLine(size_t count) {
    if (count == 0) {
        return false;
    }

    const size_t caretY = std::min(_caret.y + count, _textLayouts.size() - 1);
    const size_t caretX = std::min(_caret.xCorrector, _textLayouts[caretY]->getTextLength());
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
    setCaret(_textLayouts[_caret.y]->getTextLength(), _caret.y, true);
    return true;
}

void TextEditor::setupCursorByOffset(double x, double y) {
    const size_t caretY = getCaretYByOffset(y);
    const size_t caretX = getCaretXByOffset(caretY, x);
    setCaret(caretX, caretY, true);
}

bool TextEditor::eraseBeforeCursor(size_t count) {
    if (count == 0 || _caret.x == 0 && _caret.y == 0) {
        return false;
    }

    size_t remainingCount = count;
    if (_caret.x >= count) {
        _textLayouts[_caret.y]->eraseText(_caret.x - count, count);
        setCaret(_caret.x - count, _caret.y, true);
        setShouldCachedTextBeUpdated(true);
        return true;
    }
    assert(_caret.y > 0);
    remainingCount -= _caret.x;

    const rendell_text::String &remainingText = takeRemainingTextInLine(_caret.x, _caret.y, false);
    removeTextLayout(_caret.y);
    remainingCount--;
    size_t caretY = _caret.y - 1;
    while (remainingCount > 0 && caretY > 0) {
        if (const size_t textLength = _textLayouts[caretY]->getTextLength();
            remainingCount > textLength) {
            removeTextLayout(caretY);
            remainingCount -= textLength;
            caretY--;
        } else {
            break;
        }
    }
    const auto &currentTextLayout = _textLayouts[caretY];
    currentTextLayout->eraseText(currentTextLayout->getTextLength() - remainingCount,
                                 remainingCount);
    currentTextLayout->appendText(remainingText);
    setCaret(_textLayouts[caretY]->getTextLength() - remainingText.length(), caretY, true);
    setShouldCachedTextBeUpdated(true);

    return true;
}

bool TextEditor::eraseAfterCursor(size_t count) {
    if (count == 0) {
        return false;
    }

    _caret.xCorrector = _caret.x;

    size_t remainingCount = count;
    const auto &currentTextLayout = _textLayouts[_caret.y];
    if (const size_t textLength = currentTextLayout->getTextLength();
        _caret.x + count < textLength) {
        currentTextLayout->eraseText(_caret.x, count);
        remainingCount -= count;
    } else if (_caret.x < textLength) {
        currentTextLayout->eraseText(_caret.x, count);
        setShouldCachedTextBeUpdated(true);
        return true;
    }

    while (remainingCount > 0 && _caret.y + 1 < _textLayouts.size()) {
        const auto &removedTextLayout = _textLayouts[_caret.y + 1];
        assert(removedTextLayout);
        removeTextLayout(_caret.y + 1);

        remainingCount--;
        if (const size_t textLength = removedTextLayout->getTextLength();
            textLength < remainingCount) {
            remainingCount -= textLength;
        } else {
            if (removedTextLayout->getTextLength() > 0) {
                const rendell_text::String &remainingText =
                    takeRemainingTextInLine(*removedTextLayout.get(), remainingCount);
                // const rendell_text::String& remainingText =
                // removedTextLayout->getSubText(remainingCount);
                _textLayouts[_caret.y]->appendText(remainingText);
            }
            break;
        }
    }
    setShouldCachedTextBeUpdated(true);

    return true;
}

bool TextEditor::eraseWordBeforeCursor() {
    return eraseBeforeCursor(getPrevWordLength());
}

bool TextEditor::eraseWordAfterCursor() {
    return eraseAfterCursor(getNextWordLength());
}

bool TextEditor::eraseLineUnderCursor() {
    if (_textLayouts.size() == 1) {
        _textLayouts[0]->eraseText(0);
        setCaret(0, 0);
        setShouldCachedTextBeUpdated(true);
        return true;
    }

    removeTextLayout(_caret.y);
    const size_t caretY = std::min(_caret.y, _textLayouts.size() - 1);
    const size_t caretX = std::min(_caret.xCorrector, _textLayouts[caretY]->getTextLength());
    setCaret(caretX, caretY);
    setShouldCachedTextBeUpdated(true);
    return true;
}

bool TextEditor::insertAfterCursor(const rendell_text::String &text) {
    if (text.empty()) {
        return false;
    }

    std::vector<rendell_text::String> lines = StringExtension::split(text, U"\n");
    assert(!lines.empty());
    _textLayouts[_caret.y]->insertText(lines[0], _caret.x);

    if (lines.size() == 1) {
        setCaret(_caret.x + lines[0].length(), _caret.y, true);
        setShouldCachedTextBeUpdated(true);
        return true;
    }

    size_t caretY = _caret.y;
    const rendell_text::String &remainingText =
        takeRemainingTextInLine(_caret.x + lines[0].length(), _caret.y, true);
    for (size_t i = 1; i < lines.size(); i++) {
        auto newTextLayout = createTextLayout(std::move(lines[i]));
        caretY++;
        addTextLayout(caretY, newTextLayout);
    }
    setCaret(_textLayouts[caretY]->getTextLength(), caretY, true);
    _textLayouts[caretY]->appendText(remainingText);
    setShouldCachedTextBeUpdated(true);
    return true;
}

bool TextEditor::moveLineUnderCursorDown() {
    if (_caret.y + 1 < _textLayouts.size()) {
        swipeLines(_caret.y, _caret.y + 1);
        setCaret(_caret.x, _caret.y + 1, true);
        setShouldCachedTextBeUpdated(true);
        return true;
    }
    return false;
}

bool TextEditor::moveLineUnderCursorUp() {
    if (_caret.y > 0) {
        swipeLines(_caret.y, _caret.y - 1);
        setCaret(_caret.x, _caret.y - 1, true);
        setShouldCachedTextBeUpdated(true);
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

void TextEditor::setShouldCachedTextBeUpdated(bool value) {
    _shouldCachedTextBeUpdated = value;
    if (_shouldCachedTextBeUpdated) {
        textChanged.emit();
    }
}

rendell_text::String TextEditor::takeRemainingTextInLine(rendell_text::ITextLayout &textLayout,
                                                         size_t caretX, bool erase) {
    rendell_text::String result =
        textLayout.getTextLength() > caretX ? textLayout.getSubText(caretX) : U"";
    if (erase) {
        textLayout.eraseText(caretX);
    }
    return result;
}

rendell_text::String TextEditor::takeRemainingTextInLine(size_t caretX, size_t caretY, bool erase) {
    const auto &textLayout = _textLayouts[caretY];
    assert(textLayout);
    return takeRemainingTextInLine(*textLayout.get(), caretX, erase);
}

void TextEditor::removeTextLayout(size_t index) {
    _textLayouts.erase(_textLayouts.begin() + index);
    textLayoutRemoved.emit(index);
}

void TextEditor::addTextLayout(size_t index,
                               std::shared_ptr<rendell_text::ITextLayout> textLayout) {
    _textLayouts.insert(_textLayouts.begin() + index, textLayout);
    textLayoutAdded.emit(index, textLayout);
}

void TextEditor::swipeLines(size_t firstIndex, size_t secondIndex) {
    std::swap(_textLayouts[firstIndex], _textLayouts[secondIndex]);
    textLayoutSwapped.emit(firstIndex, secondIndex);
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

void TextEditor::updateCachedTextIfNeeded() const {
    if (_shouldCachedTextBeUpdated) {
        _cachedText = convertLinesToString();
        _shouldCachedTextBeUpdated = false;
    }
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
    size_t result = 0;
    size_t caretX = _caret.x;
    size_t caretY = _caret.y;
    if (caretX == 0 && caretY > 0) {
        caretY--;
        caretX = _textLayouts[caretY]->getTextLength();
        result++;
    }

    if (const rendell_text::String &text = _textLayouts[caretY]->getText(); text.length() > 0) {
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
    if (caretX == _textLayouts[caretY]->getTextLength() && caretY < _textLayouts.size() - 1) {
        caretY++;
        caretX = 0;
        result++;
    }

    if (const rendell_text::String &text = _textLayouts[caretY]->getText(); text.size() > 0) {
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
    assert(_textLayouts.size() > 0);

    size_t currentOffset = _textLayouts[0]->getGlyphAtlasCache()->getLineHeight() / 2;
    double distance = abs(offset - static_cast<double>(currentOffset));
    for (size_t i = 0; i < _textLayouts.size(); i++) {
        currentOffset += _textLayouts[i]->getGlyphAtlasCache()->getLineHeight();
        double newDistance = abs(offset - static_cast<double>(currentOffset));
        if (newDistance < distance) {
            distance = newDistance;
        } else {
            return i;
        }
    }

    return _textLayouts.size() - 1;
}

size_t TextEditor::getCaretXByOffset(size_t caretY, double offset) const {
    const std::vector<uint32_t> &textAdvance = _textLayouts[caretY]->getTextAdvance();
    if (textAdvance.size() == 0) {
        return 0;
    }

    size_t currentOffset = textAdvance[0] / 2;
    double distance = abs(offset - static_cast<double>(currentOffset));
    for (size_t i = 0; i < textAdvance.size(); i++) {
        currentOffset = textAdvance[i];
        double newDistance = abs(offset - static_cast<double>(currentOffset));
        if (newDistance < distance) {
            distance = newDistance;
        } else {
            return i;
        }
    }

    return textAdvance.size();
}

TextEditorWordSharedPtr TextEditor::findWord(wchar_t character) const {
    for (const TextEditorWordSharedPtr &word : _wordTypes) {
        if (word->isWordCharacter(character)) {
            return word;
        }
    }
    return nullptr;
}

rendell_text::String TextEditor::convertLinesToString() const {
    size_t resultLength = 0;
    for (const auto &textLayout : _textLayouts) {
        resultLength += textLayout->getText().length() + 1;
    }

    rendell_text::String result;
    result.reserve(resultLength);
    for (const auto &textLayout : _textLayouts) {
        result += textLayout->getText() + U"\n";
    }

    return result;
}

} // namespace rendell_ui