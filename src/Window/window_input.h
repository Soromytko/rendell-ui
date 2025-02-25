#pragma once

namespace rendell_ui
{
	enum class InputKey
	{
		space,

		A, B, C, D, E, F,
		G, H, I, J, K, L,
		M, N, O, P, Q, R,
		S, T, U, V, W, X,
		Y, Z,

		enter,
		tab,
		backspace,
		del,

		right, left, down, up,
	};

	enum class InputMouseButton
	{
		leftButton,
		middleButton,
		rightButton,
	};

	enum class InputAction
	{
		pressed,
		release,
		repeat,
	};

	enum class InputMod
	{
		alt,
		ctrl,
	};

	class InputModControl
	{
	public:
		InputModControl(int mods);

		bool hasMod(InputMod mod, int mods) const;
		bool hasModOnly(InputMod mod, int mods) const;
		bool hasCtrlMod() const;
		bool hasAltMod() const;
		bool hasShiftMod() const;

	private:
		int _mods;
	};

	struct MouseInput
	{
		InputMouseButton button;
		InputAction action;
		InputModControl modControl;
		double x, y;
	};

	struct KeyboardInput
	{
		InputKey key;
		InputAction action;
		InputModControl modControl;
	};

	InputKey convertGlfwKeyToInputKey(int key);
	InputMouseButton convertGlfwMouseButtonToInputMouseButton(int button);
	InputAction convertGlfwActionToInputAction(int action);
}
