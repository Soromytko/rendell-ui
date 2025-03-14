#pragma once
#include "Widget.h"

namespace rendell_ui
{
	class RectangleWidget : public Widget
	{
		FRIEND_WIDGET
	protected:
		RectangleWidget();
	public:
		virtual ~RectangleWidget();

		virtual void draw() const override;

	private:
		rendell::VertexArraySharedPtr _vertexArray;

	};

	DECLARE_WIDGET(RectangleWidget)

}
