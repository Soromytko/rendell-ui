#pragma once
#include <memory>
#include <rendell/rendell.h>
#include <string>

namespace rendell_ui {
class Shader {
public:
    Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
    ~Shader() = default;

    rendell::ShaderProgramSharedPtr getShaderProgram() const;

private:
    rendell::ShaderProgramSharedPtr _shaderProgram;
};
} // namespace rendell_ui