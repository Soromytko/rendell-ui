#pragma once
#include <rendell_ui/Widgets/IScrollableWidget.h>

#include <glm/glm.hpp>

namespace rendell_text {
class IGlyphAtlasCache;
}

namespace rendell_ui {
class ITextBuffer;

class TextDrawer final : public IScrollableWidget {
public:
    TextDrawer(std::shared_ptr<rendell_text::IGlyphAtlasCache> glyphAtlasCache);
    ~TextDrawer() = default;

    // IScrollableWidget
    double getScrollProgress() const override;
    double getScrollRatio() const override;
    bool setScrollProgress(double value) override;
    void onProcessMouseScrolled(glm::dvec2 scroll) override;

    uint32_t getTextHeight() const;
    glm::dvec2 getSize() const;
    double getScroll() const;
    void draw(const glm::mat4 viewMatrix, const glm::mat4 &modelMatrix);

    void setSize(glm::dvec2 value);

private:
    std::pair<const std::shared_ptr<rendell_text::ITextLayout> *, size_t> findVisibleTextLayouts();
    void updateScroll();
    void onScrollProgressChanged(float lastScrollProgress, float newScrollProgress);

    rendell::oop::ShaderBuffer _shaderBuffer;

    std::vector<std::shared_ptr<ITextBuffer>> _textBuffers{};

    size_t _startRenderingIndex{0};
    uint32_t _textHeight{0};

    glm::dvec2 _size{};
    double _scroll{};
    double _scrollProgress{};
};
} // namespace rendell_ui