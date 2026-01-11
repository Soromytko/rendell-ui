#include <TextModel.h>

#include <StringExtension.h>

#include <rendell_text/IGlyphAtlasCache.h>
#include <rendell_text/ITextLayout.h>
#include <rendell_text/factory.h>

namespace rendell_ui {
TextModel::TextModel(std::shared_ptr<rendell_text::IGlyphAtlasCache> glyphAtlasCache)
    : _glyphAtlasCache(glyphAtlasCache) {
    assert(_glyphAtlasCache);
}

const rendell_text::String &TextModel::getText() const {
    updateCachedTextIfNeeded();
    return _cachedText;
}

const rendell_text::String &TextModel::getLineText(size_t lineIndex) const {
    assert(lineIndex < _lines.size());
    return _lines[lineIndex]->getText();
}

size_t TextModel::getLineCount() const {
    return _lines.size();
}

uint32_t TextModel::getHeight() const {
    return _height;
}

uint32_t TextModel::getLineHeight(size_t lineIndex) const {
    assert(lineIndex < _lines.size());
    return _lines[lineIndex]->getHeight();
}

size_t TextModel::getLineLength(size_t lineIndex) const {
    assert(lineIndex < _lines.size());
    return _lines[lineIndex]->getTextLength();
}

uint32_t TextModel::getLineDescender(size_t lineIndex) const {
    assert(lineIndex < _lines.size());
    assert(_lines[lineIndex]);
    return _lines[lineIndex]->getDescender();
}

const std::vector<uint32_t> &TextModel::getLineAdvance(size_t lineIndex) const {
    assert(lineIndex < _lines.size());
    assert(_lines[lineIndex]);
    return _lines[lineIndex]->getTextAdvance();
}

size_t TextModel::getLineIndexByOffset(double offset) const {
    assert(_glyphAtlasCache);

    size_t currentOffset = _glyphAtlasCache->getLineHeight() / 2;
    double distance = abs(offset - static_cast<double>(currentOffset));
    for (size_t i = 0; i < _lines.size(); i++) {
        assert(_lines[i]);
        currentOffset += _lines[i]->getGlyphAtlasCache()->getLineHeight();
        double newDistance = abs(offset - static_cast<double>(currentOffset));
        if (newDistance < distance) {
            distance = newDistance;
        } else {
            return i;
        }
    }

    return _lines.size() - 1;
}

size_t TextModel::getCharIndexByOffset(size_t lineIndex, double offset) const {
    assert(lineIndex < _lines.size());

    assert(_lines[lineIndex]);
    const std::vector<uint32_t> &textAdvance = _lines[lineIndex]->getTextAdvance();
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

uint32_t TextModel::getLineOffset(size_t lineIndex) const {
    assert(lineIndex < _lines.size());
    assert(_lineOffsets.size() == _lines.size());
    return _lineOffsets[lineIndex];
}

uint32_t TextModel::getOffsetInLine(size_t lineIndex, size_t charIndex) const {
    assert(lineIndex < _lines.size());
    rendell_text::ITextLayout *line = _lines[lineIndex].get();
    assert(line);
    assert(charIndex < line->getTextLength());
    return line->getTextAdvance()[charIndex];
}

void TextModel::setText(rendell_text::String text) {
    if (!_shouldCachedTextBeUpdated && _cachedText == text) {
        return;
    }

    std::vector<rendell_text::String> lines = StringExtension::split(text, U"\n");
    const size_t oldLineCount = lines.size();
    _lines.reserve(lines.size());

    if (oldLineCount < _lines.size()) {
        for (size_t i = 0; i < oldLineCount; i++) {
            rendell_text::String lineText = std::move(lines[i]);
            _lines[i]->setText(lineText);
        }
        for (size_t i = oldLineCount; i < _lines.size(); i++) {
            rendell_text::String lineText = std::move(lines[i]);
            auto textLayout = createTextLayout(std::move(lineText));
            _lines[i] = textLayout;
        }
        return;
    }

    for (size_t i = 0; i < _lines.size(); i++) {
        rendell_text::String lineText = std::move(lines[i]);
        _lines[i]->setText(std::move(lineText));
    }

    _cachedText = std::move(text);
    _shouldCachedTextBeUpdated = false;
}

void TextModel::insertText(size_t lineIndex, size_t charIndex, const rendell_text::String &text) {
    assert(lineIndex < _lines.size());
    assert(charIndex < _lines[lineIndex]->getTextLength());
    if (text.empty()) {
        return;
    }

    std::vector<rendell_text::String> lines = StringExtension::split(text, U"\n");
    assert(!lines.empty());
    _lines[lineIndex]->insertText(lines[0], charIndex);

    if (lines.size() == 1) {
        _shouldCachedTextBeUpdated = true;
    }

    size_t caretY = lineIndex;
    const rendell_text::String &remainingText =
        takeRemainingTextInLine(charIndex + lines[0].length(), lineIndex, true);
    for (size_t i = 1; i < lines.size(); i++) {
        auto newTextLayout = createTextLayout(std::move(lines[i]));
        caretY++;
        addTextLayout(caretY, newTextLayout);
    }
    _lines[caretY]->appendText(remainingText);
    _shouldCachedTextBeUpdated = true;
}

void TextModel::appendText(size_t lineIndex, const rendell_text::String &text) {
    assert(lineIndex < _lines.size());
    assert(_lines[lineIndex]);
    if (text.empty()) {
        return;
    }
    _lines[lineIndex]->appendText(text);
}

void TextModel::insertLine(size_t lineIndex, rendell_text::String text) {
    assert(lineIndex <= _lines.size());
    auto line = createTextLayout(std::move(text));
    _lines.insert(_lines.begin() + lineIndex, line);
    _shouldCachedTextBeUpdated = true;
    if (line->isEmpty()) {
        return;
    }
    insertText(lineIndex, line->getTextLength(), text);
}

void TextModel::eraseTextForward(size_t lineIndex, size_t charIndex, size_t count) {
    assert(lineIndex < _lines.size());
    assert(_lines[lineIndex]);
    assert(charIndex < _lines[lineIndex]->getTextLength());

    if (count == 0) {
        return;
    }

    size_t remainingCount = count;
    const auto &currentTextLayout = _lines[lineIndex];
    if (const size_t textLength = currentTextLayout->getTextLength();
        charIndex + count < textLength) {
        currentTextLayout->eraseText(charIndex, count);
        remainingCount -= count;
    } else if (charIndex < textLength) {
        currentTextLayout->eraseText(charIndex, count);
        _shouldCachedTextBeUpdated = true;
        return;
    }

    while (remainingCount > 0 && lineIndex + 1 < _lines.size()) {
        const auto &removedTextLayout = _lines[lineIndex + 1];
        assert(removedTextLayout);
        removeTextLayout(lineIndex + 1);

        remainingCount--;
        if (const size_t textLength = removedTextLayout->getTextLength();
            textLength < remainingCount) {
            remainingCount -= textLength;
        } else {
            if (removedTextLayout->getTextLength() > 0) {
                const rendell_text::String &remainingText =
                    takeRemainingTextInLine(*removedTextLayout.get(), remainingCount);
                _lines[lineIndex]->appendText(remainingText);
            }
            break;
        }
    }
    _shouldCachedTextBeUpdated = true;
}

void TextModel::eraseTextBack(size_t &lineIndex, size_t &charIndex, size_t count) {
    assert(lineIndex < _lines.size());
    assert(_lines[lineIndex]);
    assert(charIndex < _lines[lineIndex]->getTextLength());

    if (count == 0 || charIndex == 0 && lineIndex == 0) {
        return;
    }

    size_t remainingCount = count;
    if (charIndex >= count) {
        _lines[lineIndex]->eraseText(charIndex - count, count);
        _shouldCachedTextBeUpdated = true;
        return;
    }
    assert(lineIndex > 0);
    remainingCount -= charIndex;

    const rendell_text::String &remainingText =
        takeRemainingTextInLine(charIndex, lineIndex, false);
    removeTextLayout(lineIndex);
    remainingCount--;
    lineIndex--;
    while (remainingCount > 0 && lineIndex > 0) {
        if (const size_t textLength = _lines[lineIndex]->getTextLength();
            remainingCount > textLength) {
            removeTextLayout(lineIndex);
            remainingCount -= textLength;
            lineIndex--;
        } else {
            break;
        }
    }
    const auto &currentTextLayout = _lines[lineIndex];
    currentTextLayout->eraseText(currentTextLayout->getTextLength() - remainingCount,
                                 remainingCount);
    currentTextLayout->appendText(remainingText);
    charIndex = _lines[lineIndex]->getTextLength() - remainingText.length();
    _shouldCachedTextBeUpdated = true;
}

void TextModel::eraseLine(size_t lineIndex) {
    assert(lineIndex < _lines.size());
    removeTextLayout(lineIndex);
}

void TextModel::swipeLines(size_t firstLineIndex, size_t secondLineIndex) {
    assert(firstLineIndex < _lines.size());
    assert(secondLineIndex < _lines.size());
    if (firstLineIndex == secondLineIndex) {
        return;
    }
    std::swap(_lines[firstLineIndex], _lines[secondLineIndex]);
}

std::shared_ptr<rendell_text::ITextLayout>
TextModel::createTextLayout(rendell_text::String &&text) {
    assert(_glyphAtlasCache);
    auto result = rendell_text::createTextLayout(_glyphAtlasCache);
    result->setText(std::move(text));
    return result;
}

rendell_text::String TextModel::takeRemainingTextInLine(rendell_text::ITextLayout &textLayout,
                                                        size_t caretX, bool erase) {
    rendell_text::String result =
        textLayout.getTextLength() > caretX ? textLayout.getSubText(caretX) : U"";
    if (erase) {
        textLayout.eraseText(caretX);
    }
    return result;
}

rendell_text::String TextModel::takeRemainingTextInLine(size_t caretX, size_t caretY, bool erase) {
    const auto &textLayout = _lines[caretY];
    assert(textLayout);
    return takeRemainingTextInLine(*textLayout.get(), caretX, erase);
}

void TextModel::removeTextLayout(size_t index) {
    assert(index < _lines.size());
    assert(_lineOffsets.size() == _lines.size());

    const auto lineHeight = _lineOffsets[index];
    _lines.erase(_lines.begin() + index);
    _lineOffsets.erase(_lineOffsets.begin() + index);
    for (size_t i = index; i < _lineOffsets.size(); i++) {
        _lineOffsets[i] -= lineHeight;
    }

    _shouldCachedTextBeUpdated = true;
}

void TextModel::addTextLayout(size_t index, std::shared_ptr<rendell_text::ITextLayout> textLayout) {
    assert(index <= _lines.size());
    assert(textLayout);
    assert(_lineOffsets.size() == _lines.size());

    _lines.insert(_lines.begin() + index, textLayout);

    const uint32_t lastLineOffset =
        index > 0 && _lineOffsets.size() > 0 ? _lineOffsets[index - 1] : 0;
    const uint32_t newLineHeight = textLayout->getHeight();
    _lineOffsets.insert(_lineOffsets.begin() + index, lastLineOffset + newLineHeight);
    for (size_t i = index + 1; i < _lineOffsets.size(); i++) {
        _lineOffsets[i] += newLineHeight;
    }

    _shouldCachedTextBeUpdated = true;
}

void TextModel::updateCachedTextIfNeeded() const {
    if (_shouldCachedTextBeUpdated) {
        _cachedText = convertLinesToString();
        _shouldCachedTextBeUpdated = false;
    }
}

rendell_text::String TextModel::convertLinesToString() const {
    size_t resultLength = 0;
    for (const auto &textLayout : _lines) {
        resultLength += textLayout->getText().length() + 1;
    }

    rendell_text::String result;
    result.reserve(resultLength);
    for (const auto &textLayout : _lines) {
        result += textLayout->getText() + U"\n";
    }

    return result;
}

} // namespace rendell_ui