#include "input.h"
#include <GLFW/glfw3.h>

namespace rendell_ui
{
	InputAction convertGlfwActionToInputAction(int action)
	{
		switch (action)
		{
		case GLFW_PRESS: return InputAction::pressed;
		case GLFW_RELEASE: return InputAction::release;
		case GLFW_REPEAT: return InputAction::repeat;
		}
	}
}
