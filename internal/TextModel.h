#pragma once
#include <rendell_ui/ITextModel.h>

#include <memory>
#include <vector>

namespace rendell_text {
class ITextLayout;
class IGlyphAtlasCache;
} // namespace rendell_text

namespace rendell_ui {
class TextModel final : public ITextModel {
public:
    TextModel(std::shared_ptr<rendell_text::IGlyphAtlasCache> glyphAtlasCache);
    ~TextModel() = default;

    const rendell_text::String &getText() const override;
    const rendell_text::String &getLineText(size_t lineIndex) const override;
    size_t getLineCount() const override;
    uint32_t getHeight() const override;
    uint32_t getLineHeight(size_t lineIndex) const override;
    size_t getLineLength(size_t lineIndex) const override;
    uint32_t getLineDescender(size_t lineIndex) const override;
    const std::vector<uint32_t> &getLineAdvance(size_t lineIndex) const override;
    size_t getLineIndexByOffset(double offset) const override;
    size_t getCharIndexByOffset(size_t lineIndex, double offset) const override;
    uint32_t getLineOffset(size_t lineIndex) const override;
    uint32_t getOffsetInLine(size_t lineIndex, size_t charIndex) const override;

    void setText(rendell_text::String text) override;
    void insertText(size_t lineIndex, size_t charIndex, const rendell_text::String &text) override;
    void appendText(size_t lineIndex, const rendell_text::String &text) override;
    void insertLine(size_t lineIndex, rendell_text::String text = {}) override;
    void eraseTextForward(size_t lineIndex, size_t charIndex, size_t count) override;
    void eraseTextBack(size_t &lineIndex, size_t &charIndex, size_t count) override;
    void eraseLine(size_t lineIndex) override;
    void swipeLines(size_t firstLineIndex, size_t secondLineIndex) override;

private:
    std::shared_ptr<rendell_text::ITextLayout> createTextLayout(rendell_text::String &&text);

    rendell_text::String takeRemainingTextInLine(rendell_text::ITextLayout &textLayout,
                                                 size_t caretX, bool erase = false);
    rendell_text::String takeRemainingTextInLine(size_t caretX, size_t caretY, bool erase = false);

    void removeTextLayout(size_t index);
    void addTextLayout(size_t index, std::shared_ptr<rendell_text::ITextLayout> textLayout);

    void updateCachedTextIfNeeded() const;

    rendell_text::String convertLinesToString() const;

    std::shared_ptr<rendell_text::IGlyphAtlasCache> _glyphAtlasCache;
    uint32_t _height;
    std::vector<std::shared_ptr<rendell_text::ITextLayout>> _lines{};
    std::vector<uint32_t> _lineOffsets{};

    mutable rendell_text::String _cachedText{};
    mutable bool _shouldCachedTextBeUpdated{};
};

} // namespace rendell_ui