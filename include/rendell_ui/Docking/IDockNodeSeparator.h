#pragma once
#include <functional>
#include <glm/glm.hpp>
#include <rendell_ui/defines.h>

namespace rendell_ui {
class IDockNodeSeparator {
public:
    using OnDraggedCallback = std::function<void(float ratio)>;

    IDockNodeSeparator() = default;
    virtual ~IDockNodeSeparator() = default;

    virtual bool getIsHorizontal() const = 0;
    virtual float getRatio() const = 0;
    virtual float getThickness() const = 0;

    virtual void setIsHorizontal(bool value) = 0;
    virtual void setRatio(float value) = 0;
    virtual void setThickness(float value) = 0;
    virtual void setDockNodeRect(glm::vec2 dockNodeOrigin, glm::vec2 dockNodeSize) = 0;
    virtual void setOnDraggedCallback(OnDraggedCallback callback) = 0;

    virtual void release() = 0;
};

RENDELL_UI_DECLARE_SHARED_PTR(IDockNodeSeparator)

} // namespace rendell_ui