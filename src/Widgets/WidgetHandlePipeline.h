#pragma once
#include <set>
#include "IWidgetCollection.h"

namespace rendell_ui
{
	class WidgetHandlePipeline : public IWidgetCollection
	{
	public:
		WidgetHandlePipeline() = default;
		~WidgetHandlePipeline() = default;

		void append(IWidget* widget) override;
		void remove(IWidget* widget) override;

	private:
		std::set<IWidget*> _widgets{};
	};

	DECLARE_SHARED_PTR_FACTORY(WidgetHandlePipeline)
}
