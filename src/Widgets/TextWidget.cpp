#include <rendell_ui/Widgets/TextWidget.h>

#include <FontStorage.h>
#include <TextModel.h>
#include <rendell_text/ITextLayout.h>
#include <rendell_ui/IFont.h>
#include <rendell_ui/Viewport.h>

#include <cassert>

namespace rendell_ui {
static std::shared_ptr<ITextModel> createDefaultTextModel() {
    auto defaultGlyphAtlasCache = FontStorage::getInstance()->getDefaultGlyphAtlasCache();
    assert(defaultGlyphAtlasCache);
    return std::make_shared<TextModel>(defaultGlyphAtlasCache);
}

TextWidget::TextWidget()
    : Widget()
    , _textModel([]() {
        auto result = createDefaultTextModel();
        assert(result);
        return result;
    }())
    , _textDrawer({_textModel}) {
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

void TextWidget::setText(rendell_text::String value) {
    assert(_textModel);
    _textModel->setText(std::move(value));
}

std::shared_ptr<IFont> TextWidget::getFont() const {
    return _font;
}

uint32_t TextWidget::getFontSize() const {
    return _fontSize;
}

const rendell_text::String &TextWidget::getText() const {
    assert(_textModel);
    return _textModel->getText();
}

// void TextWidget::updateText() {
//     std::vector<rendell_text::String> lines = StringExtension::split(_text, U"\n");
//     const size_t oldLineCount = lines.size();
//     _textLayouts.reserve(lines.size());
//
//     if (oldLineCount < _textLayouts.size()) {
//         for (size_t i = 0; i < oldLineCount; i++) {
//             rendell_text::String lineText = std::move(lines[i]);
//             _textLayouts[i]->setText(lineText);
//         }
//         for (size_t i = oldLineCount; i < _textLayouts.size(); i++) {
//             rendell_text::String lineText = std::move(lines[i]);
//             auto textLayout = createTextLayout(std::move(lineText));
//             auto textRenderer = createTextRenderer(textLayout);
//             _textLayouts[i] = textRenderer;
//         }
//         return;
//     }
//
//     for (size_t i = 0; i < _textLayouts.size(); i++) {
//         rendell_text::String lineText = std::move(lines[i]);
//         _textLayouts[i]->setText(std::move(lineText));
//     }
// }

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

    _textDrawer.draw(viewMat, transformMat);
}
} // namespace rendell_ui
