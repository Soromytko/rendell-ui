#pragma once

namespace rendell_ui
{
	class Time final
	{
		//friend void updateTime(double time);
	private:
		Time() = default;

	public:
		~Time() = default;

		static void updateDeltaTime();
		static double getDeltaTime();
		static double getCurrentTime();
	};
}