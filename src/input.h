#pragma once

namespace rendell_ui
{
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

	InputAction convertGlfwActionToInputAction(int action);
}
