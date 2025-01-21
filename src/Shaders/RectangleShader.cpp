#include "RectangleShader.h"

static const std::string s_vertexSrc = R"(
	#version 330 core

	layout(location = 0) in vec2 a_VertexPosition;
	uniform mat4 u_Matrix;
	uniform vec2 u_Size;

	void main()
	{
		gl_Position = u_Matrix * vec4(a_VertexPosition * u_Size, 0.0, 1.0);
	}
)";

static const std::string s_fragmentSrc = R"(
	#version 330 core

	out vec4 o_Color;
	uniform vec4 u_Color;

	void main()
	{
		o_Color = u_Color;
	}
)";

RectangleShader::RectangleShader() : Shader(s_vertexSrc, s_fragmentSrc)
{
	
}