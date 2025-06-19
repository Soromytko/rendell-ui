#include "Shader.h"
#include <logging.h>

namespace rendell_ui
{
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		_shaderProgram = rendell::createShaderProgram(vertexSrc, fragmentSrc);
		std::string vertexInfoLog, fragmentInfoLog;
		if (!_shaderProgram->compile(&vertexInfoLog, &fragmentInfoLog))
		{
			RUI_ERROR("Shader compilation error:\n{}\n{}", vertexInfoLog, fragmentInfoLog);
			exit(-1);
		}
		std::string linkInfoLog;
		if (!_shaderProgram->link(&linkInfoLog))
		{
			RUI_ERROR("Shader linking error:\n{}", linkInfoLog);
			exit(-1);
		}
		_shaderProgram->freeSrc();

	}

	rendell::ShaderProgramSharedPtr Shader::getShaderProgram() const
	{
		return _shaderProgram;
	}
}
