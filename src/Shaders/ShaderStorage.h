#pragma once
#include "RectangleShader.h"
#include "Shader.h"
#include <memory>

namespace rendell_ui {
class ShaderStorage final {
private:
    ShaderStorage();

public:
    ~ShaderStorage() = default;

    static void init();
    static void release();
    static ShaderStorage *getInstance();

    const RectangleShaderUniquePtr rectangleShader;
};
} // namespace rendell_ui
