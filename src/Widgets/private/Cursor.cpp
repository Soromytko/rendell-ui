#include <rendell_ui/Widgets/private/Cursor.h>
#include "../../Time.h"

#define CURSOR_THICKNESS 2.0f

namespace rendell_ui
{
	Cursor::Cursor() : RectangleWidget()
	{
		setThickness(CURSOR_THICKNESS);
		_timer = makeTimer(500);
		_timer->triggered.connect([this]() { _shouldDraw = !_shouldDraw; });
		_timer->start();
	}

	void Cursor::draw() const
	{
		if (_shouldDraw)
		{
			RectangleWidget::draw();
		}
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

	void Cursor::setBlinkTimeout(size_t ms)
	{
		_timer->setInterval(ms);
	}

	void Cursor::resetBlinkTimer()
	{
		_shouldDraw = true;
		if (_timer->isRunning())
		{
			_timer->stop();
			_timer->start();
		}
	}

	size_t Cursor::getBlinkTimeout() const
	{
		return _timer->getInterval();
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

	void Cursor::onVisibleChanged()
	{
		if (_visible)
		{
			_timer->start();
		}
		else
		{
			_timer->stop();
		}
	}
}