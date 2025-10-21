#include "../Shaders/ShaderStorage.h"
#include <array>
#include <cassert>
#include <rendell_ui/Viewport.h>
#include <rendell_ui/Widgets/RectangleWidget.h>

namespace rendell_ui {
static const std::vector<float> s_vertices{
    -0.5f, +0.5f, -0.5f, -0.5f, +0.5f, +0.5f, +0.5f, -0.5f,
};

static const std::vector<uint32_t> s_indices{0, 0, 0, 0};

RectangleWidget::RectangleWidget()
    : Widget() {
    setName("RectangleWidget");
    _shaderProgram = ShaderStorage::getInstance()->rectangleShader->getShaderProgram();
    auto indexBuffer = rendell::oop::makeIndexBuffer(s_indices.data(), s_indices.size());
    auto vertexBuffer = rendell::oop::makeVertexBuffer(s_vertices.data(), s_vertices.size());
    auto layout =
        rendell::VertexLayout().addAttribute(0, rendell::ShaderDataType::float2, false, 0);
    _vertexAssembly = rendell::oop::makeVertexAssembly(indexBuffer, std::vector{vertexBuffer},
                                                       std::vector{layout});
}

RectangleWidget::~RectangleWidget() {
}

void RectangleWidget::draw() {
    updateUniforms();

    _vertexAssembly->use();
    _shaderProgram->use();

    rendell::submit(rendell::DrawMode::Arrays, rendell::PrimitiveTopology::TriangleStrip);
}
} // namespace rendell_ui
