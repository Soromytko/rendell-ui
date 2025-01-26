#include "Cursor.h"
#include "../../Time.h"

namespace rendell_ui
{
	Cursor::Cursor(Widget* parent) : Rectangle(parent)
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

		Rectangle::draw();

	}

	void Cursor::moveTo(glm::vec2 value)
	{
		_offset = value;
		_blinkTimer = _blinkTimeout;
		updateRecursively();
	}

	void Cursor::moveByDelta(glm::vec2 delta)
	{
		moveTo(_offset + delta);
	}
}