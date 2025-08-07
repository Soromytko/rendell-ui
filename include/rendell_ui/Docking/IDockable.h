#pragma once
#include <glm/glm.hpp>
#include <rendell_ui/defines.h>

namespace rendell_ui {
class IDockable {
public:
    using MinSizeChangedCallback = std::function<void(glm::vec2 minSize)>;
    using HeaderDraggedCallback = std::function<void()>;

    IDockable() = default;
    virtual ~IDockable() = default;

    virtual glm::vec2 getSize() const = 0;
    virtual glm::vec2 getMinSize() const = 0;
    virtual glm::vec2 getPosition() const = 0;
    virtual MinSizeChangedCallback getMinSizeChangedCallback() const = 0;
    virtual HeaderDraggedCallback getHeaderDraggedCallback() const = 0;

    virtual void setSize(glm::vec2 value) = 0;
    virtual void setMinSize(glm::vec2 value) = 0;
    virtual void setPosition(glm::vec2 value) = 0;
    virtual void setMinSizeChangedCallback(MinSizeChangedCallback callback) = 0;
    virtual void setHeaderDraggedCallback(HeaderDraggedCallback callback) = 0;
};

RENDELL_UI_DECLARE_SHARED_PTR(IDockable)
RENDELL_UI_DECLARE_WEAK_PTR(IDockable)

} // namespace rendell_ui
