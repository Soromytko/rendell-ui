#pragma once
#include "Shader.h"

namespace rendell_ui
{
	class RectangleShader final : public Shader
	{
	public:
		RectangleShader();
		~RectangleShader() = default;
	};

	typedef std::unique_ptr<RectangleShader> RectangleShaderUniquePtr;
}

