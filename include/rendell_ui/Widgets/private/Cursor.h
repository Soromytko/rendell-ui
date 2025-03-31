#pragma once
#include <rendell_ui/Widgets/RectangleWidget.h>

namespace rendell_ui
{
	class Cursor final : public RectangleWidget
	{
	public:
		Cursor();
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

	RENDELL_UI_DECLARE_WIDGET(Cursor)

}
