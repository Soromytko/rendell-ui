#include "window_input.h"
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

	InputKey convertGlfwKeyToInputKey(int key)
	{
		switch (key)
		{
		case GLFW_KEY_SPACE: return InputKey::space;
		case GLFW_KEY_A: return InputKey::a;
		case GLFW_KEY_B: return InputKey::b;
		case GLFW_KEY_C: return InputKey::c;
		case GLFW_KEY_D: return InputKey::d;
		case GLFW_KEY_E: return InputKey::e;
		case GLFW_KEY_F: return InputKey::f;
		case GLFW_KEY_V: return InputKey::v;
		case GLFW_KEY_W: return InputKey::w;
		case GLFW_KEY_X: return InputKey::x;
		case GLFW_KEY_ENTER: return InputKey::enter;
		case GLFW_KEY_TAB: return InputKey::tab;
		case GLFW_KEY_BACKSPACE: return InputKey::backspace;
		case GLFW_KEY_DELETE: return InputKey::del;
		case GLFW_KEY_RIGHT: return InputKey::right;
		case GLFW_KEY_LEFT: return InputKey::left;
		case GLFW_KEY_DOWN: return InputKey::down;
		case GLFW_KEY_UP: return InputKey::up;
		}
	}

	InputMouseButton convertGlfwMouseButtonToInputMouseButton(int button)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT: return InputMouseButton::leftButton;
		case GLFW_MOUSE_BUTTON_MIDDLE: return InputMouseButton::middleButton;
		case GLFW_MOUSE_BUTTON_RIGHT: return InputMouseButton::rightButton;
		}
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
