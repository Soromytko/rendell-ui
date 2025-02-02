#pragma once
#include "../Rectangle.h"

namespace rendell_ui
{
	class Cursor final : public Rectangle
	{
		friend class TextRendererEditor;

	protected:
		Cursor(Widget* parent = nullptr);

	public:
		~Cursor() = default;

		void draw() const override;

		void setThickness(float value);
		void setVerticalOffset(float value);
		void moveTo(glm::vec2 value);
		void moveByDelta(glm::vec2 delta);

		float getThickness() const;
		float getVerticalOffset() const;

	private:
		float _verticalOffset{ 0.0f };
		double _blinkTimeout{ 0.5f };
		mutable double _blinkTimer{ 0.0f };
	};

}
