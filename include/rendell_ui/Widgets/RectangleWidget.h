#pragma once
#include "Widget.h"

namespace rendell_ui {
class RectangleWidget : public Widget {
public:
    RectangleWidget();
    virtual ~RectangleWidget();

    virtual void draw() override;

private:
    rendell::oop::VertexAssemblySharedPtr _vertexAssembly;
};

RENDELL_UI_DECLARE_WIDGET(RectangleWidget)

} // namespace rendell_ui
