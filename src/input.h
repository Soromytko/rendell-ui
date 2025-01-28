#pragma once

namespace rendell_ui
{
	enum class InputAction
	{
		pressed,
		release,
		repeat,
	};

	InputAction convertGlfwActionToInputAction(int action);
}
