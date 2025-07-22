#pragma once
#include "Widget.h"
#include <rendell_text/rendell_text.h>

namespace rendell_ui {
class TextWidget final : public Widget {
public:
    TextWidget();
    ~TextWidget() = default;

    void setColor(glm::vec4 value) override;
    void draw() const override;

    void setFontPath(const std::string &value);
    void setFontSize(glm::ivec2 value);
    void setBackgroundColor(glm::vec4 value);
    void setText(const std::wstring &value);

    const std::filesystem::path &getFontPath() const;
    glm::ivec2 getFontSize() const;
    // const glm::vec4& getBackgroundColor() const;
    const std::wstring &getText() const;

private:
    void updateText();
    rendell_text::TextLayoutSharedPtr createTextLayout(std::wstring &&text) const;
    rendell_text::TextRendererSharedPtr createTextRenderer(std::wstring &&text) const;

    glm::vec4 _backgroundColor = glm::vec4(31.0f / 255.0, 31.0f / 255.0, 31.0f / 255.0, 1.0f);
    glm::ivec2 _fontSize{24, 24};
    std::filesystem::path _fontPath{};
    std::vector<rendell_text::TextRendererSharedPtr> _lines{};
    std::wstring _text{};
};

RENDELL_UI_DECLARE_WIDGET(TextWidget)
} // namespace rendell_ui
