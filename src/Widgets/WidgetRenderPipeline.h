#pragma once
#include <vector>
#include <memory>
#include "IWidgetCollection.h"
#include "../defines.h"

namespace rendell_ui
{
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

	DECLARE_SHARED_PTR_FACTORY(WidgetRenderPipeline)
}
