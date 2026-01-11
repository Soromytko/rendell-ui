#include <rendell_ui/Widgets/private/TextDrawer.h>

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
TextDrawer::TextDrawer(std::shared_ptr<ITextModel> textModel)
    : _textModel(textModel) {
    assert(_textModel);

#error
    _textRenderer = rendell_text::createTextRenderer(nullptr, nullptr);
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

bool TextDrawer::isVisibleLine(const TextLine &textLine) const {
    return true;
    /*  assert(_textModel);
      const uint32_t lineHeight = [&]() {
          assert(textLine.textRenderer);
          assert(textLine.textRenderer->getTextLayout());
          return textLine.textRenderer->getTextLayout()->getHeight();
      }();
      const float viewYOffset = static_cast<float>(_textModel->getHeight()) * _scrollProgress;
      const float viewHeight = _size.y;

      return textLine.yOffset + static_cast<float>(lineHeight) < viewYOffset + viewHeight;*/
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

    // Remove invisible lines
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

    // Add new lines
    auto [newVisibleTextLayouts, count] = findVisibleTextLayouts();
    if (_visibleTextLayouts.size() == 0) {
        _visibleTextLayouts.insert(_visibleTextLayouts.end(), newVisibleTextLayouts,
                                   newVisibleTextLayouts + count);
    } else if (lastScrollProgress < newScrollProgress) {
        assert(_visibleTextLayouts[0]);
        auto targetTextLayout =
            std::find(newVisibleTextLayouts, newVisibleTextLayouts + count, _visibleTextLayouts[0]);
        const size_t countToCopy = static_cast<size_t>(targetTextLayout - newVisibleTextLayouts);
        _visibleTextLayouts.insert(_visibleTextLayouts.begin(), newVisibleTextLayouts,
                                   newVisibleTextLayouts + countToCopy);
    } else {
        for (size_t i = count - 1; i != 0; i--) {
            auto textLayout = newVisibleTextLayouts[i];
            assert(textLayout);
            if (textLayout != _visibleTextLayouts[_visibleTextLayouts.size() - 1]) {
                continue;
            }
            _visibleTextLayouts.insert(_visibleTextLayouts.end(), newVisibleTextLayouts + i,
                                       newVisibleTextLayouts + count);
            break;
        }
    }

    std::shared_ptr<rendell_text::ITextBuffer> currentTextBuffer;
    size_t offset = 0;
    for (auto &textLayout : _visibleTextLayouts) {
        while (true) {
            const size_t remaining = currentTextBuffer->update(offset, textLayout);
            if (remaining == 0) {
                offset += textLayout->getTextLength();
                break;
            }
            offset = 0;
            // get nextTextBuffer;
        }
    }

    const size_t visibleTextLength = std::accumulate(
        _visibleTextLayouts.begin(), _visibleTextLayouts.end(), size_t(0),
        [](size_t accum, const std::shared_ptr<rendell_text::ITextLayout> &textLayout) {
            return accum + textLayout->getTextLength();
        });
    const size_t textBufferSize = 500;
    const size_t textBufferCount = (visibleTextLength + textBufferSize - 1) / textBufferSize;

    _textBuffers.resize(std::max(textBufferCount, _textBuffers.size()));
    for (size_t i = 0; i < textBufferCount; i++) {
        std::shared_ptr<rendell_text::ITextBuffer> &textBuffer = _textBuffers[i];
        if (!textBuffer) {
            textBuffer = rendell_text::createTextBuffer();
        }
        assert(textBuffer);
        textBuffer->update(_textBuffer);
    }
}

} // namespace rendell_ui