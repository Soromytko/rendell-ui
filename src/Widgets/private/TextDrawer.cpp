#include <rendell_ui/Widgets/private/TextDrawer.h>

#include <rendell_text/IGlyphAtlasCache.h>
#include <rendell_text/ITextBuffer.h>

#include <rendell_text/ITextLayout.h>
#include <rendell_text/ITextRenderer.h>
#include <rendell_text/factory.h>
#include <rendell_ui/ITextModel.h>
#include <rendell_ui/Viewport.h>

#include <glm/gtc/matrix_transform.hpp>

#include <cassert>
#include <numeric>

namespace rendell_ui {
TextDrawer::TextDrawer(std::shared_ptr<rendell_text::IGlyphAtlasCache> glyphAtlasCache)
    : _glyphAtlasCache(glyphAtlasCache) {
}

double TextDrawer::getScrollProgress() const {
    return _scrollProgress;
}

double TextDrawer::getScrollRatio() const {
    return _size.y / static_cast<double>(_textHeight);
}

bool TextDrawer::setScrollProgress(double value) {
    if (_scrollProgress != value) {
        const double oldValue = _scrollProgress;
        _scrollProgress = value;
        onScrollProgressChanged(oldValue, value);
        updateScroll();
        return true;
    }
    return false;
}

void TextDrawer::onProcessMouseScrolled(glm::dvec2 scroll) {
}

uint32_t TextDrawer::getTextHeight() const {
    return _textHeight;
}

glm::dvec2 TextDrawer::getSize() const {
    return _size;
}

double TextDrawer::getScroll() const {
    return _scroll;
}

// const std::vector<rendell_text::TextRendererSharedPtr> &TextDrawer::getLines() const {
//     return _textRenderers;
// }

void TextDrawer::draw(const glm::mat4 viewMatrix, const glm::mat4 &modelMatrix) {
    const glm::vec3 offset = modelMatrix[3];

    Viewport::getCurrent()->setScissors(static_cast<int>(offset.x), static_cast<int>(offset.y),
                                        static_cast<int>(_size.x), static_cast<int>(_size.y));

    for (std::shared_ptr<rendell_text::ITextBuffer> &textBuffer : _textBuffers) {
        _textRenderer->setTextBuffer(textBuffer);
        _textRenderer->setTexture
    }

    for (const TextLine &textLine : _lines) {
        if (!textLine.isVisible) {
            continue;
        }
        assert(textLine.textRenderer);
        const glm::mat4 worldMat =
            glm::translate(modelMatrix, glm::vec3(-_size.x * 0.5f, textLine.yOffset, 0.0f));
        textLine.textRenderer->setMatrix(viewMatrix * worldMat);
        textLine.textRenderer->draw();
    }
}

void TextDrawer::setSize(glm::dvec2 value) {
    if (_size != value) {
        _size = value;
        updateScroll();
    }
}

void TextDrawer::updateScroll() {
    _scroll = (static_cast<double>(_textHeight) - _size.y) * _scrollProgress;
    _scroll = std::max(0.0, _scroll);
}

void TextDrawer::onScrollProgressChanged(float lastScrollProgress, float newScrollProgress) {
}

} // namespace rendell_ui