#pragma once
#include <vector>
#include <functional>
#include <mutex>
#include <rendell_ui/defines.h>

namespace rendell_ui
{
	class ActionPool final
	{
	public:
		using Action = std::function<void()>;

		ActionPool() = default;
		~ActionPool() = default;

		size_t getActionCount() const;

		void addAction(Action action);
		void execute();
		void clear();

	private:
		std::mutex _mutex;
		std::vector<Action> _actions;
	};


	RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(ActionPool)
}