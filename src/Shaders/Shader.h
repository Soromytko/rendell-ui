#pragma once
#include <memory>
#include <rendell/oop/rendell_oop.h>
#include <string>

namespace rendell_ui {
class Shader {
public:
    Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
    ~Shader() = default;

    rendell::oop::ShaderProgramSharedPtr getShaderProgram() const;

private:
    rendell::oop::ShaderProgramSharedPtr _shaderProgram;
};
} // namespace rendell_ui