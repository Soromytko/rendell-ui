#pragma once
#pragma once
#include "TextEditorWord.h"
#include <rendell_text/types.h>
#include <rendell_ui/Signal.h>

#include <memory>

namespace rendell_text {
class ITextLayout;
class IGlyphAtlasCache;
} // namespace rendell_text

namespace rendell_ui {
class TextEditor final {
    struct Caret {
        size_t x{};
        size_t y{};
        size_t xCorrector{};
        size_t offsetInString{};
    };

public:
    TextEditor(std::shared_ptr<rendell_text::IGlyphAtlasCache> glyphAtlasCache);
    ~TextEditor() = default;

    const rendell_text::String &getText() const;
    size_t getCaretOffsetInString() const;
    uint32_t getCursorVerticalOffset() const;
    uint32_t getCursorHorizontalOffset() const;
    uint32_t getCursorHeight() const;
    const std::vector<TextEditorWordSharedPtr> &getWordTypes() const;

    void setText(rendell_text::String &&text);
    void setWordTypes(const std::vector<TextEditorWordSharedPtr> &value);

    bool moveCursorToPrevChar(size_t count = 1);
    bool moveCursorToNextChar(size_t count = 1);
    bool moveCursorToPrevLine(size_t count = 1);
    bool moveCursorToNextLine(size_t count = 1);
    bool moveCursorToPrevWord();
    bool moveCursorToNextWord();
    bool moveCursorToStart();
    bool moveCursorToEnd();
    void setupCursorByOffset(double x, double y);

    bool eraseBeforeCursor(size_t count = 1);
    bool eraseAfterCursor(size_t count = 1);
    bool eraseWordBeforeCursor();
    bool eraseWordAfterCursor();
    bool eraseLineUnderCursor();
    bool insertAfterCursor(const rendell_text::String &text);
    bool moveLineUnderCursorDown();
    bool moveLineUnderCursorUp();

    Signal<void> textChanged;
    Signal<void> textLayoutCleared;
    Signal<void, size_t> textLayoutRemoved;
    Signal<void, size_t, const rendell_text::TextLayoutSharedPtr &> textLayoutAdded;
    Signal<void, size_t, size_t> textLayoutSwapped;
    Signal<void, uint32_t, uint32_t, uint32_t> cursorChanged;

private:
    std::shared_ptr<rendell_text::ITextLayout> createTextLayout(rendell_text::String &&text);
    void setShouldCachedTextBeUpdated(bool value);

    rendell_text::String takeRemainingTextInLine(rendell_text::ITextLayout &textLayout,
                                                 size_t caretX, bool erase = false);
    rendell_text::String takeRemainingTextInLine(size_t caretX, size_t caretY, bool erase = false);

    void removeTextLayout(size_t index);
    void addTextLayout(size_t index, std::shared_ptr<rendell_text::ITextLayout> textLayout);
    void swipeLines(size_t firstIndex, size_t secondIndex);
    bool setCaret(size_t x, size_t y, bool setXCorrector = false);

    void updateCachedTextIfNeeded() const;

    bool isSameWord(const TextEditorWordSharedPtr &word, wchar_t character) const;
    size_t getPrevWordLength() const;
    size_t getNextWordLength() const;
    size_t getCaretYByOffset(double offset) const;
    size_t getCaretXByOffset(size_t caretY, double offset) const;

    TextEditorWordSharedPtr findWord(wchar_t character) const;
    rendell_text::String convertLinesToString() const;

    Caret _caret{};
    std::vector<std::shared_ptr<rendell_text::ITextLayout>> _textLayouts;
    std::vector<TextEditorWordSharedPtr> _wordTypes;
    std::shared_ptr<rendell_text::IGlyphAtlasCache> _glyphAtlasCache;
    mutable rendell_text::String _cachedText{};
    mutable bool _shouldCachedTextBeUpdated{};
};

} // namespace rendell_ui