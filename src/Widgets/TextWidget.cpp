#include "../String/StringExtension.h"
#include <glm/gtc/matrix_transform.hpp>
#include <rendell_ui/Viewport.h>
#include <rendell_ui/Widgets/TextWidget.h>

namespace rendell_ui {
TextWidget::TextWidget()
    : Widget() {
    setName("TextWidget");
    _fontPath = std::filesystem::path(FONT_ROOT_DIRECTORY) / "mononoki/mononoki-Regular.ttf";
}

void TextWidget::setFontPath(const std::string &value) {
    if (_fontPath != value) {
        _fontPath = value;
        updateText();
    }
}

void TextWidget::setFontSize(glm::ivec2 value) {
    if (_fontSize != value) {
        _fontSize = value;
        for (const rendell_text::TextRendererSharedPtr &line : _lines) {
            line->getTextLayout()->setFontSize(_fontSize);
        }
    }
}

void TextWidget::setBackgroundColor(glm::vec4 value) {
    if (_backgroundColor != value) {
        _backgroundColor = value;
        for (const rendell_text::TextRendererSharedPtr &line : _lines) {
            line->setBackgroundColor(_backgroundColor);
        }
    }
}

void TextWidget::setText(const std::wstring &value) {
    if (_text != value) {
        _text = value;
        updateText();
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

void TextWidget::updateText() {
    _lines.clear();

    std::vector<std::wstring> lines = StringExtension::split(_text, L"\n");
    _lines.reserve(lines.size());
    for (std::wstring &line : lines) {
        const rendell_text::TextRendererSharedPtr &textRenderer =
            createTextRenderer(std::move(line));
        _lines.push_back(textRenderer);
    }
}

rendell_text::TextLayoutSharedPtr TextWidget::createTextLayout(std::wstring &&text) const {
    rendell_text::TextLayoutSharedPtr result = rendell_text::makeTextLayout();
    result->setText(std::move(text));
    result->setFontPath(_fontPath);
    result->setFontSize(_fontSize);
    return result;
}

rendell_text::TextRendererSharedPtr TextWidget::createTextRenderer(std::wstring &&text) const {
    rendell_text::TextLayoutSharedPtr textLayout = createTextLayout(std::move(text));
    rendell_text::TextRendererSharedPtr result = rendell_text::makeTextRenderer();
    result->setTextLayout(textLayout);
    result->setBackgroundColor(_backgroundColor);
    result->setColor(_color);
    return result;
}

void TextWidget::setColor(glm::vec4 value) {
    if (_color != value) {
        Widget::setColor(value);
        for (const rendell_text::TextRendererSharedPtr &line : _lines) {
            line->setColor(_color);
        }
    }
}

void TextWidget::draw() {
    const glm::mat4 &projectMat = Viewport::getCurrent()->getProjectMat();
    const glm::mat4 &viewMat = Viewport::getCurrent()->getViewMat();
    const glm::mat4 &transformMat = _transform.getMatrix();

    float currentOffset = _size.y * 0.5f;
    for (const rendell_text::TextRendererSharedPtr &line : _lines) {
        currentOffset -= line->getTextLayout()->getFontHeight();
        const glm::mat4 worldMat =
            glm::translate(transformMat, glm::vec3(-_size.x * 0.5f, currentOffset, 0.0f));
        line->setMatrix(projectMat * viewMat * worldMat);
        line->draw();
    }
}
} // namespace rendell_ui
