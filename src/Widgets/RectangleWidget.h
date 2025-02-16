#pragma once
#include "Widget.h"

namespace rendell_ui
{
	class Rectangle : public Widget
	{
	public:
		Rectangle(Widget* parent = nullptr);
		virtual ~Rectangle();

		virtual void draw() const override;

	private:
		rendell::VertexArraySharedPtr _vertexArray;

	};

	typedef std::shared_ptr<Rectangle> RectangleSharedPtr;
}
