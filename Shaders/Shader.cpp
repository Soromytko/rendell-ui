#include "Shader.h"

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	_shaderProgram = rendell::createShaderProgram(vertexSrc, fragmentSrc);
	std::string vertexInfoLog, fragmentInfoLog;
	if (!_shaderProgram->compile(&vertexInfoLog, &fragmentInfoLog))
	{
		std::cout << "ERROR::Shader: Compilation error:\n" << vertexInfoLog << "\n" << fragmentInfoLog << std::endl;
		exit(-1);
	}
	std::string linkInfoLog;
	if (!_shaderProgram->link(&linkInfoLog))
	{
		std::cout << "ERROR::Shader: Linking error:\n" << linkInfoLog << std::endl;
		exit(-1);
	}
	_shaderProgram->freeSrc();

}

rendell::ShaderProgramSharedPtr Shader::getShaderProgram() const
{
	return _shaderProgram;
}
