#include <glm/gtc/matrix_transform.hpp>
#include <rendell_text/ITextLayout.h>
#include <rendell_text/ITextRenderer.h>
#include <rendell_text/factory.h>
#include <rendell_ui/Viewport.h>
#include <rendell_ui/Widgets/private/TextDrawer.h>

#include <rendell_ui/ITextModel.h>

#include <cassert>

namespace rendell_ui {
TextDrawer::TextDrawer(std::shared_ptr<ITextModel> textModel)
    : _textModel(textModel) {
    assert(_textModel);
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

    for (TextLine &textLine : _lines) {
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

void TextDrawer::clear() {
    /*   _textRenderers.clear();
       _textHeight = 0;
       _startRenderingIndex = 0;*/
}

void TextDrawer::removeLine(size_t index) {
    // auto it = _textRenderers.begin() + index;
    //_textHeight -= it->operator->()->getTextLayout()->getFontHeight();
    //_textRenderers.erase(it);
    // optimizeRendering();
}

// void TextDrawer::addLine(size_t index, const rendell_text::TextRendererSharedPtr &textRenderer) {
/* _textRenderers.insert(_textRenderers.begin() + index, textRenderer);
 _textHeight += textRenderer->getTextLayout()->getFontHeight();
 optimizeRendering();*/
//}

void TextDrawer::swapLines(size_t firstIndex, size_t secondIndex) {
    // std::swap(_textRenderers[firstIndex], _textRenderers[secondIndex]);
}

bool TextDrawer::isVisibleLine(const TextLine &textLine) const {
    assert(_textModel);
    const uint32_t lineHeight = [&]() {
        assert(textLine.textRenderer);
        assert(textLine.textRenderer->getTextLayout());
        return textLine.textRenderer->getTextLayout()->getHeight();
    }();
    const float viewYOffset = static_cast<float>(_textModel->getHeight()) * _scrollProgress;
    const float viewHeight = _size.y;

    return textLine.yOffset + static_cast<float>(lineHeight) < viewYOffset + viewHeight;
}

std::pair<const std::shared_ptr<rendell_text::ITextLayout> *, size_t>
TextDrawer::findVisibleTextLayouts() {
    size_t count = 0;

    return {_visibleTextLayoutContainer.data(), count};
}

void TextDrawer::updateScroll() {
    _scroll = (static_cast<double>(_textHeight) - _size.y) * _scrollProgress;
    _scroll = std::max(0.0, _scroll);
}

void TextDrawer::onScrollProgressChanged(float lastScrollProgress, float newScrollProgress) {
    assert(_textModel);

    // Remove anvisibles.
    if (lastScrollProgress < newScrollProgress) {
        auto it = std::find_if(_lines.rbegin(), _lines.rend(),
                               [this](const std::shared_ptr<TextLine> &line) {
                                   assert(line);
                                   return isVisibleLine(*line.get());
                               });
        _lines.erase(it.base(), _lines.end());
    } else {
        auto it = std::find_if(_lines.begin(), _lines.end(),
                               [this](const std::shared_ptr<TextLine> &line) {
                                   assert(line);
                                   return isVisibleLine(*line.get());
                               });
        _lines.erase(_lines.begin(), it);
    }

    auto [textLayouts, count] = findVisibleTextLayouts();
    if (_lines.size() == 0) {
        _lines.reserve(count);
        for (size_t i = 0; i < count; i++) {
            assert(textLayouts[i]);
            _lines.push_back({
                .textRenderer = _textRendererPool.pull(textLayouts[i]),
                .yOffset = 0,
                .isVisible = false,
            });
        }
    } else if (lastScrollProgress < newScrollProgress) {
        for (size_t i = 0; i < count; i++) {
            auto textLayout = textLayouts[i];
            assert(textLayout);
            if (textLayout != _lines[0].textRenderer->getTextLayout()) {
                continue;
            }
            for (size_t j = 0; j < i; j++) {
                _lines.insert(_lines.begin() + j,
                              {
                                  .textRenderer = _textRendererPool.pull(textLayouts[j]),
                                  .yOffset = 0,
                                  .isVisible = true,
                              });
            }
            assert(_lines[i].textRenderer->getTextLayout() == textLayout);
            break;
            /*   if (_lines[i].textRenderer->getTextLayout() != textLayout) {
                   _lines.insert(_lines.begin(),
                                 {
                                     .textRenderer = _textRendererPool.pull(textLayout),
                                     .yOffset = 0,
                                     .isVisible = false,
                                 });
               } else {
                   break;
               }*/
        }
    } else {
        for (size_t i = count - 1; i != 0; i--) {
            auto textLayout = textLayouts[i];
            assert(textLayout);
            if (textLayout != _lines[_lines.size() - 1].textRenderer->getTextLayout()) {
                continue;
            }
            for (size_t j = i; j < count; j++) {
                _lines.push_back({
                    .textRenderer = _textRendererPool.pull(textLayouts[j]),
                    .yOffset = 0,
                    .isVisible = true,
                });
            }
            break;
        }
    }
}

std::shared_ptr<rendell_text::ITextRenderer>
TextDrawer::TextRendererPool::pull(std::shared_ptr<rendell_text::ITextLayout> textLayout) {
    assert(textLayout);
    return rendell_text::createTextRenderer(textLayout);
}

void TextDrawer::TextRendererPool::push(std::shared_ptr<rendell_text::ITextRenderer> textRenderer) {
}

} // namespace rendell_ui