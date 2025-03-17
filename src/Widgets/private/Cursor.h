#pragma once
#include "../RectangleWidget.h"

namespace rendell_ui
{
	class Cursor final : public RectangleWidget
	{
		FRIEND_WIDGET
	private:
		Cursor();

	public:
		~Cursor() = default;

		void draw() const override;

		void setThickness(float value);
		void setHeight(float value);
		void setVerticalOffset(float value);
		void resetBlinkTimer();

		float getThickness() const;
		float getHeight() const;
		float getVerticalOffset() const;

	private:
		float _verticalOffset{ 0.0f };
		double _blinkTimeout{ 0.5f };
		mutable double _blinkTimer{ 0.0f };
	};

	DECLARE_WIDGET(Cursor)

}
