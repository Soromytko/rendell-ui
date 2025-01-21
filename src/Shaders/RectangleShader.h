#pragma once
#include "Shader.h"

class RectangleShader final : public Shader
{
public:
	RectangleShader();
	~RectangleShader() = default;
};

typedef std::unique_ptr<RectangleShader> RectangleShaderUniquePtr;
