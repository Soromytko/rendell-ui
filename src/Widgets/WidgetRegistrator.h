#pragma once
#include <vector>
#include <rendell_ui/Widgets/IWidget.h>
#include "IWidgetCollection.h"

namespace rendell_ui
{
	class WidgetRegistrator final
	{
	private:
		WidgetRegistrator(std::initializer_list<IWidgetCollectionSharedPtr> collections);

	public:
		~WidgetRegistrator() = default;

		static void init(std::initializer_list<IWidgetCollectionSharedPtr> collections);
		static void release();
		static bool hasInstance();
		static WidgetRegistrator* getInstance();

		void registerWidget(IWidget* widget);
		void unregisterWidget(IWidget* widget);

	private:
		std::vector<IWidgetCollectionSharedPtr> _collections;

	};
}
