#pragma once
#include "Widget.h"

namespace rendell_text {
class IGlyphAtlasCache;
class ITextLayout;
class ITextRenderer;
} // namespace rendell_text

namespace rendell_ui {
class IFont;

class TextWidget final : public Widget {
public:
    TextWidget();
    ~TextWidget() = default;

    void setColor(glm::vec4 value) override;
    void draw() override;

    void setFont(std::shared_ptr<IFont> font);
    void setFontSize(uint32_t value);
    void setBackgroundColor(glm::vec4 value);
    void setText(const rendell_text::String &value);

    std::shared_ptr<IFont> getFont() const;
    uint32_t getFontSize() const;
    // const glm::vec4& getBackgroundColor() const;
    const rendell_text::String &getText() const;

private:
    void updateText();
    rendell_text::TextLayoutSharedPtr createTextLayout(rendell_text::String &&text) const;
    rendell_text::TextRendererSharedPtr
    createTextRenderer(std::shared_ptr<rendell_text::TextLayout> textLayout) const;

    glm::vec4 _backgroundColor = glm::vec4(31.0f / 255.0, 31.0f / 255.0, 31.0f / 255.0, 1.0f);
    uint32_t _fontSize{24};
    std::shared_ptr<IFont> _font{};
    std::shared_ptr<rendell_text::IGlyphAtlasCache> _glyphAtlasCache{};
    std::vector<std::unique_ptr<rendell_text::ITextLayout>> _textLayouts{};
    std::vector<std::shared_ptr<rendell_text::ITextRenderer>> _visibleTextLines{};
    rendell_text::String _text{};
};

RENDELL_UI_DECLARE_WIDGET(TextWidget)
} // namespace rendell_ui
