#include "../String/StringExtension.h"
#include <glm/gtc/matrix_transform.hpp>
#include <rendell_ui/Viewport.h>
#include <rendell_ui/Widgets/TextWidget.h>

#include <algorithm>

namespace rendell_ui {
TextWidget::TextWidget()
    : Widget() {
    setName("TextWidget");
    _fontPath = std::filesystem::path(FONT_ROOT_DIRECTORY) / "mononoki/mononoki-Regular.ttf";
}

void TextWidget::setFontPath(const std::string &value) {
    if (_fontPath != value) {
        _fontPath = value;
        for (rendell_text::TextLayout &line : _lines) {
            line.setFontPath(value);
        }
    }
}

void TextWidget::setFontSize(glm::ivec2 value) {
    if (_fontSize != value) {
        _fontSize = value;
        for (rendell_text::TextLayout &line : _lines) {
            line.setFontSize(_fontSize);
        }
    }
}

void TextWidget::setBackgroundColor(glm::vec4 value) {
    if (_backgroundColor != value) {
        _backgroundColor = value;
        _textRenderer.setBackgroundColor(value);
    }
}

void TextWidget::setText(const std::wstring &value) {
    if (_text == value) {
        return;
    }
    _text = value;

    std::vector<std::wstring> lines = StringExtension::split(_text, L"\n");
    const bool isTextIncreased = lines.size() > _lines.size();
    _lines.resize(lines.size());

    for (size_t i = 0; i < lines.size(); i++) {
        std::wstring lineText = std::move(lines[i]);
        rendell_text::TextLayout &textLayout = _lines[i];
        textLayout.beginUpdating();
        textLayout.setText(std::move(lineText));
        if (isTextIncreased) {
            textLayout.setFontPath(_fontPath);
            textLayout.setFontSize(_fontSize);
        }
        textLayout.endUpdating();
    }
}

const std::filesystem::path &TextWidget::getFontPath() const {
    return _fontPath;
}

glm::ivec2 TextWidget::getFontSize() const {
    return _fontSize;
}

const std::wstring &TextWidget::getText() const {
    return _text;
}

void TextWidget::setColor(glm::vec4 value) {
    if (_color != value) {
        Widget::setColor(value);
        _textRenderer.setColor(value);
    }
}

void TextWidget::draw() {
    const glm::mat4 &projectMat = Viewport::getCurrent()->getProjectMat();
    const glm::mat4 &viewMat = Viewport::getCurrent()->getViewMat();
    const glm::mat4 &transformMat = _transform.getMatrix();

    float currentOffset = _size.y * 0.5f;
    for (rendell_text::TextLayout &line : _lines) {
        currentOffset -= line.getFontHeight();
        const glm::mat4 worldMat =
            glm::translate(transformMat, glm::vec3(-_size.x * 0.5f, currentOffset, 0.0f));
        _textRenderer.setMatrix(projectMat * viewMat * worldMat);
        _textRenderer.draw(line);
    }
}
} // namespace rendell_ui
