#pragma once
#include <set>
#include "IWidgetCollection.h"

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