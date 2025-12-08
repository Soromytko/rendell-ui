#include <rendell_ui/Widgets/TextWidget.h>

#include "../String/StringExtension.h"
#include <FontStorage.h>
#include <rendell_text/ITextLayout.h>
#include <rendell_text/ITextRenderer.h>
#include <rendell_ui/IFont.h>
#include <rendell_ui/Viewport.h>

#include <glm/gtc/matrix_transform.hpp>

namespace rendell_ui {
TextWidget::TextWidget()
    : Widget() {
    setName("TextWidget");
    auto font = FontStorage::getInstance()->getDefaultFont();
    setFont(font);
}

void TextWidget::setFont(std::shared_ptr<IFont> font) {
    assert(font);
    if (_font != font) {
        _font = font;
        auto glyphAtlasCache = FontStorage::getInstance()->getOrCreateGlyphAtlasCache(*_font.get());
        if (_glyphAtlasCache != glyphAtlasCache) {
            _glyphAtlasCache = glyphAtlasCache;
            for (auto &textLayout : _textLayouts) {
                textLayout->setGlyphAtlasCache(_glyphAtlasCache);
            }
        }
    }
}

void TextWidget::setFontSize(uint32_t value) {
    _fontSize = value;
}

void TextWidget::setBackgroundColor(glm::vec4 value) {
    if (_backgroundColor != value) {
        _backgroundColor = value;
        for (const auto &line : _visibleTextLines) {
            line->setBackgroundColor(_backgroundColor);
        }
    }
}

void TextWidget::setText(const rendell_text::String &value) {
    if (_text != value) {
        _text = value;
        updateText();
    }
}

std::shared_ptr<IFont> TextWidget::getFont() const {
    return _font;
}

uint32_t TextWidget::getFontSize() const {
    return _fontSize;
}

const rendell_text::String &TextWidget::getText() const {
    return _text;
}

void TextWidget::updateText() {
    std::vector<rendell_text::String> lines = StringExtension::split(_text, U"\n");
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
            auto textRenderer = createTextRenderer(textLayout);
            _textLayouts[i] = textRenderer;
        }
        return;
    }

    for (size_t i = 0; i < _textLayouts.size(); i++) {
        rendell_text::String lineText = std::move(lines[i]);
        _textLayouts[i]->setText(std::move(lineText));
    }
}

rendell_text::TextLayoutSharedPtr TextWidget::createTextLayout(rendell_text::String &&text) const {
    assert(_glyphAtlasCache);
    rendell_text::TextLayoutSharedPtr result = rendell_text::makeTextLayout(_glyphAtlasCache);
    result->setText(std::move(text));
    return result;
}

rendell_text::TextRendererSharedPtr
TextWidget::createTextRenderer(std::shared_ptr<rendell_text::TextLayout> textLayout) const {
    assert(textLayout);
    rendell_text::TextRendererSharedPtr result = rendell_text::makeTextRenderer();
    result->setTextLayout(textLayout);
    result->setBackgroundColor(_backgroundColor);
    result->setColor(_color);
    return result;
}

void TextWidget::setColor(glm::vec4 value) {
    if (_color != value) {
        Widget::setColor(value);
        for (const auto &line : _visibleTextLines) {
            line->setColor(_color);
        }
    }
}

void TextWidget::draw() {
    const glm::mat4 &projectMat = Viewport::getCurrent()->getProjectMat();
    const glm::mat4 &viewMat = Viewport::getCurrent()->getViewMat();
    const glm::mat4 &transformMat = _transform.getMatrix();

    float currentOffset = _size.y * 0.5f;
    for (const auto &line : _visibleTextLines) {
        currentOffset -= line->getTextLayout()->getHeight();
        const glm::mat4 worldMat =
            glm::translate(transformMat, glm::vec3(-_size.x * 0.5f, currentOffset, 0.0f));
        line->setMatrix(projectMat * viewMat * worldMat);
        line->draw();
    }
}
} // namespace rendell_ui
