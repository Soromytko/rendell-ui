#pragma once
#include <rendell_ui/Widgets/IScrollableWidget.h>

#include <glm/glm.hpp>

namespace rendell_text {
class ITextLayout;
class ITextBuffer;
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

private:
    struct TextLine final {
        std::shared_ptr<rendell_text::ITextRenderer> textRenderer;
        float yOffset;
        bool isVisible;
    };

 /*   class TextBufferPool final {
    public:
        std::shared_ptr<rendell_text::ITextBuffer> pull();
        void push(std::shared_ptr<rendell_text::ITextBuffer> textBuffer);

        void collectGarbage();

    private:
        std::unordered_map<void *, std::weak_ptr<rendell_text::ITextBuffer>> _buffers;
    };*/

    //class Region final {
    //public:
    //    Region(uint32_t height, uint32_t offset, TextBufferPool &textBufferPool);
    //    ~Region();

    //private:
    //    TextBufferPool &_textBufferPool;

    //    uint32_t _offset;
    //    uint32_t _height;

    //    using TextBufferList = std::vector<std::shared_ptr<rendell_text::ITextBuffer>>;
    //    TextBufferList _textBuffers;
    //};

    //TextBufferPool _textBufferPool{};

    bool isVisibleLine(const TextLine &textLine) const;

    std::pair<const std::shared_ptr<rendell_text::ITextLayout> *, size_t> findVisibleTextLayouts();
    void updateScroll();
    void onScrollProgressChanged(float lastScrollProgress, float newScrollProgress);

    rendell::oop::ShaderBuffer _shaderBuffer;

    std::shared_ptr<ITextModel> _textModel;
    std::vector<TextLine> _lines;
    std::vector<std::shared_ptr<rendell_text::ITextLayout>> _visibleTextLayouts{};
    std::vector<std::shared_ptr<rendell_text::ITextBuffer>> _textBuffers{};
    std::vector<std::shared_ptr<rendell_text::ITextLayout>> _visibleTextLayoutContainer{};
    std::shared_ptr<rendell_text::ITextRenderer> _textRenderer;

    size_t _startRenderingIndex{0};
    uint32_t _textHeight{0};

    glm::dvec2 _size{};
    double _scroll{};
    double _scrollProgress{};
};
} // namespace rendell_ui