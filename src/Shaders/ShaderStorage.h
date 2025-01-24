#pragma once
#include <memory>
#include "Shader.h"
#include "RectangleShader.h"

namespace rendell_ui
{
	class ShaderStorage final
	{
	private:
		ShaderStorage();

	public:
		~ShaderStorage() = default;

		static void init();
		static void release();
		static ShaderStorage* getInstance();

		const RectangleShaderUniquePtr rectangleShader;

	};
}
