#pragma once
#include <rendell_text/types.h>

#include <cstdint>

namespace rendell_text {
class ITextLayout;
} // namespace rendell_text

namespace rendell_ui {
class ITextModel {
public:
    ITextModel() = default;
    virtual ~ITextModel() = default;

    virtual const rendell_text::String &getText() const = 0;
    virtual const rendell_text::String &getLineText(size_t lineIndex) const = 0;
    virtual size_t getLineCount() const = 0;
    virtual uint32_t getHeight() const = 0;
    virtual uint32_t getLineHeight(size_t lineIndex) const = 0;
    virtual size_t getLineLength(size_t lineIndex) const = 0;
    virtual uint32_t getLineDescender(size_t lineIndex) const = 0;
    virtual const std::vector<uint32_t> &getLineAdvance(size_t lineIndex) const = 0;
    virtual size_t getLineIndexByOffset(double offset) const = 0;
    virtual size_t getCharIndexByOffset(size_t lineIndex, double offset) const = 0;
    virtual uint32_t getLineOffset(size_t lineIndex) const = 0;
    virtual uint32_t getOffsetInLine(size_t lineIndex, size_t charIndex) const = 0;

    virtual void setText(rendell_text::String text) = 0;
    virtual void insertText(size_t lineIndex, size_t charIndex,
                            const rendell_text::String &text) = 0;
    virtual void appendText(size_t lineIndex, const rendell_text::String &text) = 0;
    virtual void insertLine(size_t lineIndex, rendell_text::String text = {}) = 0;
    virtual void eraseTextForward(size_t lineIndex, size_t charIndex, size_t count) = 0;
    virtual void eraseTextBack(size_t &lineIndex, size_t &charIndex, size_t count) = 0;
    virtual void eraseLine(size_t lineIndex) = 0;
    virtual void swipeLines(size_t firstLineIndex, size_t secondLineIndex) = 0;
};
} // namespace rendell_ui