#pragma once
#include <memory>
#include <string>
#include <rendell/rendell.h>

class Shader
{
public:
	Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
	~Shader() = default;

	rendell::ShaderProgramSharedPtr getShaderProgram() const;

private:
	rendell::ShaderProgramSharedPtr _shaderProgram;

};