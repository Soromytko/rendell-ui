#pragma once
#include "../RectangleWidget.h"

namespace rendell_ui
{
	class Cursor final : public RectangleWidget
	{
		FRIEND_WIDGET
		friend class TextRendererEditor;
	protected:
		Cursor();

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

	DECLARE_WIDGET(Cursor)

}
