#include <rendell_ui/Widgets/private/Cursor.h>
#include "../../Time.h"

#define CURSOR_THICKNESS 2.0f

namespace rendell_ui
{
	Cursor::Cursor() : RectangleWidget()
	{
		setThickness(CURSOR_THICKNESS);
	}

	void Cursor::draw() const
	{
		_blinkTimer += Time::getDeltaTime();
		if (_blinkTimer < _blinkTimeout)
		{
			return;
		}
		if (_blinkTimer > _blinkTimeout * 2.0)
		{
			_blinkTimer -= _blinkTimeout * 2.0;
			return;
		}

		RectangleWidget::draw();

	}

	void Cursor::setThickness(float value)
	{
		_size.x = value;
	}

	void Cursor::setHeight(float value)
	{
		_size.y = value;
	}

	void Cursor::setVerticalOffset(float value)
	{
		_verticalOffset = value;
		_offset.y = _verticalOffset;
	}

	void Cursor::resetBlinkTimer()
	{
		_blinkTimer = _blinkTimeout;
	}

	float Cursor::getThickness() const
	{
		return _size.x;
	}

	float Cursor::getHeight() const
	{
		return _size.y;
	}

	float Cursor::getVerticalOffset() const
	{
		return _verticalOffset;
	}
}