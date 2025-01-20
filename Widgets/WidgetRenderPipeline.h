#pragma once
#include <vector>
#include <memory>
#include "IWidgetCollection.h"

class WidgetRenderPipeline : public IWidgetCollection
{
public:
	WidgetRenderPipeline() = default;
	~WidgetRenderPipeline() = default;

	void append(IWidget* widget) override;
	void remove(IWidget* widget) override;

	void draw() const;

private:
	std::vector<IWidget*> _widgets{};
};

typedef std::shared_ptr<WidgetRenderPipeline> WidgetRenderPipelineSharedPtr;
