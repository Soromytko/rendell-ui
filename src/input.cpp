#include "input.h"
#include <GLFW/glfw3.h>

namespace rendell_ui
{
	static int convertCustomModToGlfwMod(InputMod mod)
	{
		switch (mod)
		{
		case InputMod::alt: return GLFW_MOD_ALT;
		case InputMod::ctrl: return GLFW_MOD_CONTROL;
		}

		return 0;
	}

	InputAction convertGlfwActionToInputAction(int action)
	{
		switch (action)
		{
		case GLFW_PRESS: return InputAction::pressed;
		case GLFW_RELEASE: return InputAction::release;
		case GLFW_REPEAT: return InputAction::repeat;
		}
	}

	InputModControl::InputModControl(int mods) : _mods(mods)
	{
	}

	bool InputModControl::hasMod(InputMod mod, int mods) const
	{
		return mods & convertCustomModToGlfwMod(mod);
	}

	bool InputModControl::hasModOnly(InputMod mod, int mods) const
	{
		return mods == convertCustomModToGlfwMod(mod);
	}

	bool InputModControl::hasCtrlMod() const
	{
		return _mods & GLFW_MOD_CONTROL;
	}

	bool InputModControl::hasAltMod() const
	{
		return _mods & GLFW_MOD_ALT;
	}

	bool InputModControl::hasShiftMod() const
	{
		return _mods & GLFW_MOD_SHIFT;
	}
}
