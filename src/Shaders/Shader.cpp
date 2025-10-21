#include "Shader.h"
#include <logging.h>

namespace rendell_ui {
Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {
    auto vertexShader =
        rendell::oop::makeVertexShader(vertexSrc, [](bool success, const std::string &infoLog) {
            if (!success) {
                RUI_ERROR("Vertex shader compilation error:\n{}", infoLog);
                exit(-1);
            }
            if (!infoLog.empty()) {
                RUI_INFO("Vertex shader compilation log:\n{}", infoLog);
            }
        });
    const auto fragmentShader =
        rendell::oop::makeFragmentShader(fragmentSrc, [](bool success, const std::string &infoLog) {
            if (!success) {
                RUI_ERROR("Fragment shader compilation error:\n{}", infoLog);
                exit(-1);
            }
            if (!infoLog.empty()) {
                RUI_INFO("Fragment shader compilation log:\n{}", infoLog);
            }
        });

    _shaderProgram = rendell::oop::makeShaderProgram(
        vertexShader, fragmentShader, [](bool success, const std::string &infoLog) {
            if (!success) {
                RUI_ERROR("Shader program linking error:\n{}", infoLog);
                exit(-1);
            }
            if (!infoLog.empty()) {
                RUI_INFO("Shader program linking log:\n{}", infoLog);
            }
        });
}

rendell::oop::ShaderProgramSharedPtr Shader::getShaderProgram() const {
    return _shaderProgram;
}
} // namespace rendell_ui
