#pragma once
#include "Widget.h"

namespace rendell_ui
{
	class RectangleWidget : public Widget
	{
		RENDELL_UI_FRIEND_WIDGET
	protected:
		RectangleWidget();
	public:
		virtual ~RectangleWidget();

		virtual void draw() const override;

	private:
		rendell::VertexArraySharedPtr _vertexArray;

	};

	RENDELL_UI_DECLARE_WIDGET(RectangleWidget)

}
