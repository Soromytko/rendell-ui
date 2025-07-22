#pragma once
#include <glm/glm.hpp>
#include <rendell_text/rendell_text.h>
#include <rendell_ui/Widgets/IScrollableWidget.h>

namespace rendell_ui {
class TextDrawer final : public IScrollableWidget {
public:
    TextDrawer();
    ~TextDrawer() = default;

    glm::vec2 getSize() const;
    const std::vector<rendell_text::TextRendererSharedPtr> &getLines() const;

    void setSize(glm::vec2 value);

private:
    glm::vec2 _size{};
    glm::mat4 _matrix{};
    std::vector<rendell_text::TextRendererSharedPtr> _lines;
};
} // namespace rendell_ui