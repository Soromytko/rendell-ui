#pragma once
#include "Widget.h"

namespace rendell_ui
{
	class RectangleWidget : public Widget
	{
	public:
		RectangleWidget(Widget* parent = nullptr);
		virtual ~RectangleWidget();

		virtual void draw() const override;

	private:
		rendell::VertexArraySharedPtr _vertexArray;

	};

	DECLARE_SHARED_PTR_FACTORY(RectangleWidget)
}
