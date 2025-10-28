#include <glm/gtc/matrix_transform.hpp>
#include <rendell_ui/Viewport.h>
#include <rendell_ui/Widgets/private/TextDrawer.h>

namespace rendell_ui {
TextDrawer::TextDrawer() {
}

double TextDrawer::getScrollProgress() const {
    return _scrollProgress;
}

double TextDrawer::getScrollRatio() const {
    return _size.y / static_cast<double>(_textHeight);
}

bool TextDrawer::setScrollProgress(double value) {
    const double newScrollProgress = _textHeight < _size.y ? 0 : std::clamp(value, 0.0, 1.0);
    if (_scrollProgress != newScrollProgress || true) {
        _scrollProgress = newScrollProgress;
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

const std::vector<rendell_text::TextLayout> &TextDrawer::getLines() const {
    return _textLayouts;
}

void TextDrawer::draw(const glm::mat4 viewMatrix, const glm::mat4 &modelMatrix) {
    const glm::vec3 offset = modelMatrix[3];

    Viewport::getCurrent()->setScissors(static_cast<int>(offset.x), static_cast<int>(offset.y),
                                        static_cast<int>(_size.x), static_cast<int>(_size.y));

    double currentOffset = _size.y * 0.5 + _startRenderingOffset;
    for (size_t i = _startRenderingIndex; i < _textLayouts.size(); i++) {
        rendell_text::TextLayout &textLayout = _textLayouts[i];
        currentOffset -= static_cast<double>(textLayout.getFontHeight());
        if (currentOffset < -_size.y) {
            break;
        }
        const glm::mat4 worldMat =
            glm::translate(modelMatrix, glm::vec3(-_size.x * 0.5f, currentOffset, 0.0f));
        _textRenderer.setMatrix(viewMatrix * worldMat);
        _textRenderer.draw(textLayout);
    }
}

void TextDrawer::setSize(glm::dvec2 value) {
    if (_size != value) {
        _size = value;
        updateScroll();
    }
}

void TextDrawer::clear() {
    _textLayouts.clear();
    _textHeight = 0;
    _startRenderingIndex = 0;
    _startRenderingOffset = 0.0f;
}

void TextDrawer::removeLine(size_t index) {
    auto it = _textLayouts.begin() + index;
    _textHeight -= it->getFontHeight();
    _textLayouts.erase(it);
    optimizeRendering();
}

void TextDrawer::addLine(size_t index, const rendell_text::TextLayout &textLayout) {
    _textLayouts.insert(_textLayouts.begin() + index, textLayout);
    _textHeight += textLayout.getFontHeight();
    optimizeRendering();
}

void TextDrawer::swapLines(size_t firstIndex, size_t secondIndex) {
    std::swap(_textLayouts[firstIndex], _textLayouts[secondIndex]);
}

void TextDrawer::updateScroll() {
    _scroll = (static_cast<double>(_textHeight) - _size.y) * _scrollProgress;
    _scroll = std::max(0.0, _scroll);
    optimizeRendering();
}

void TextDrawer::optimizeRendering() {
    const double baseOffset = _size.y * 0.5;
    double currentOffset = baseOffset + _scroll;
    for (size_t i = 0; i < _textLayouts.size(); i++) {
        _startRenderingOffset = currentOffset - baseOffset;
        currentOffset -= _textLayouts[i].getFontHeight();
        if (currentOffset <= _size.y * 0.5f) {
            _startRenderingIndex = i;
            return;
        }
    }
}

} // namespace rendell_ui