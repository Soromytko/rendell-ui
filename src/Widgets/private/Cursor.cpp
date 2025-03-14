#include "Cursor.h"
#include "../../Time.h"

namespace rendell_ui
{
	Cursor::Cursor() : RectangleWidget()
	{

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

	void Cursor::setVerticalOffset(float value)
	{
		_verticalOffset = value;
		_offset.y = _verticalOffset;
	}

	void Cursor::moveTo(glm::vec2 value)
	{
		_offset = value + glm::vec2(0.0f, _verticalOffset);
		_blinkTimer = _blinkTimeout;
		updateRecursively();
	}

	void Cursor::moveByDelta(glm::vec2 delta)
	{
		moveTo(_offset + delta - glm::vec2(0.0f, _verticalOffset));
	}

	float Cursor::getThickness() const
	{
		return _size.x;
	}

	float Cursor::getVerticalOffset() const
	{
		return _verticalOffset;
	}
}