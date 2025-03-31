#pragma once
#include "Widget.h"

namespace rendell_ui
{
	class RectangleWidget : public Widget
	{
	public:
		RectangleWidget();
		virtual ~RectangleWidget();

		virtual void draw() const override;

	private:
		rendell::VertexArraySharedPtr _vertexArray;

	};

	RENDELL_UI_DECLARE_WIDGET(RectangleWidget)

}
