#pragma once
#include "Widget.h"
#include <rendell_ui/Widgets/private/TextDrawer.h>

#include <memory>

namespace rendell_ui {
class IFont;
class ITextModel;

class TextWidget final : public Widget {
public:
    TextWidget();
    ~TextWidget() = default;

    void setColor(glm::vec4 value) override;
    void draw() override;

    void setFont(std::shared_ptr<IFont> font);
    void setFontSize(uint32_t value);
    void setBackgroundColor(glm::vec4 value);
    void setText(rendell_text::String value);

    std::shared_ptr<IFont> getFont() const;
    uint32_t getFontSize() const;
    // const glm::vec4& getBackgroundColor() const;
    const rendell_text::String &getText() const;

private:
    glm::vec4 _backgroundColor = glm::vec4(31.0f / 255.0, 31.0f / 255.0, 31.0f / 255.0, 1.0f);
    uint32_t _fontSize{24};
    std::shared_ptr<IFont> _font{};
    std::shared_ptr<rendell_text::IGlyphAtlasCache> _glyphAtlasCache{};
    std::vector<std::unique_ptr<rendell_text::ITextLayout>> _textLayouts{};
    std::vector<std::shared_ptr<rendell_text::ITextRenderer>> _visibleTextLines{};

    std::shared_ptr<ITextModel> _textModel;
    TextDrawer _textDrawer;
};

RENDELL_UI_DECLARE_WIDGET(TextWidget)
} // namespace rendell_ui
