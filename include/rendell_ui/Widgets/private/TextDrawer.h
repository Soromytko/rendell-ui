#pragma once
#include <glm/glm.hpp>
#include <rendell_ui/Widgets/IScrollableWidget.h>

namespace rendell_text {
class ITextLayout;
class ITextRenderer;
} // namespace rendell_text

namespace rendell_ui {
class ITextModel;

class TextDrawer final : public IScrollableWidget {
public:
    TextDrawer(std::shared_ptr<ITextModel> textModel);
    ~TextDrawer() = default;

    // IScrollableWidget
    double getScrollProgress() const override;
    double getScrollRatio() const override;
    bool setScrollProgress(double value) override;
    void onProcessMouseScrolled(glm::dvec2 scroll) override;

    uint32_t getTextHeight() const;
    glm::dvec2 getSize() const;
    double getScroll() const;
    // const std::vector<rendell_text::TextRendererSharedPtr> &getLines() const;
    void draw(const glm::mat4 viewMatrix, const glm::mat4 &modelMatrix);

    void setSize(glm::dvec2 value);

    void clear();
    void removeLine(size_t index);
    // void addLine(size_t index, const rendell_text::TextRendererSharedPtr &textRenderer);
    void swapLines(size_t firstIndex, size_t secondIndex);

private:
    struct TextLine final {
        std::shared_ptr<rendell_text::ITextRenderer> textRenderer;
        float yOffset;
        bool isVisible;
    };

    class TextRendererPool final {
    public:
        std::shared_ptr<rendell_text::ITextRenderer>
        pull(std::shared_ptr<rendell_text::ITextLayout> textLayout);
        void push(std::shared_ptr<rendell_text::ITextRenderer> textRenderer);
    };

    TextRendererPool _textRendererPool{};

    bool isVisibleLine(const TextLine &textLine) const;

    std::pair<const std::shared_ptr<rendell_text::ITextLayout> *, size_t> findVisibleTextLayouts();
    void updateScroll();
    void onScrollProgressChanged(float lastScrollProgress, float newScrollProgress);

    std::shared_ptr<ITextModel> _textModel;

    std::vector<std::shared_ptr<rendell_text::ITextLayout>> _visibleTextLayoutContainer{};

    size_t _startRenderingIndex{0};
    uint32_t _textHeight{0};

    glm::dvec2 _size{};
    double _scroll{};
    double _scrollProgress{};
    std::vector<std::shared_ptr<rendell_text::ITextRenderer>> _visibleTextRenderers{};
    std::vector<TextLine> _lines;
};
} // namespace rendell_ui