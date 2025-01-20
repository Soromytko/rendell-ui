#include "Widget.h"
#include "../Viewport.h"
#include "WidgetRegistrator.h"

static uint32_t s_instanceCount{ 0 };

Widget::Widget(Widget* parent) : IWidget()
{
	setParent(parent);

	WidgetRegistrator::getInstance()->registerWidget(this);
}

Widget::~Widget()
{
	WidgetRegistrator::getInstance()->unregisterWidget(this);

	for (Widget* widget : _children)
	{
		delete widget;
	}
}

void Widget::setParent(Widget* widget)
{
	if (_parent == widget || widget == this)
	{
		return;
	}
	if (_parent)
	{
		_parent->_children.erase(this);
	}
	if (widget)
	{
		widget->_children.insert(this);
	}
	_parent = widget;
}

Widget* Widget::getParent() const
{
	return _parent;
}

const std::set<Widget*>& Widget::getChildren() const
{
	return _children;
}

const Transform2D& Widget::getTransform() const
{
	return _transform;
}

void Widget::setColor(glm::vec4 value)
{
	_color = value;
}

glm::vec4 Widget::getColor() const
{
	return _color;
}

void Widget::setOffset(glm::vec2 value)
{
	_offset = value;
}

glm::vec2 Widget::getOffset() const
{
	return _offset;
}

void Widget::setSize(glm::vec2 value)
{
	_size = value;
	updateRecursively();
}

glm::vec2 Widget::getSize() const
{
	return _size;
}

void Widget::setMargins(Margins value)
{
	_margins = value;
	updateRecursively();
}

void Widget::setMargins(float left, float right, float bottom, float top)
{
	setMargins({ left, right, bottom, top });
}

Margins Widget::getMargins() const
{
	return _margins;
}

void Widget::setAnchor(Anchor value)
{
	_anchor = value;
}

Anchor Widget::getAnchor() const
{
	return _anchor;
}

void Widget::updateUniforms() const
{
	const glm::mat4& projectMat = Viewport::getCurrent()->getProjectMat();
	const glm::mat4& viewMat = Viewport::getCurrent()->getViewMat();
	const glm::mat4& worldMat = _transform.getMatrix();
	const glm::mat4 mat = projectMat * viewMat * worldMat;

	_shaderProgram->setUniformMat4(
		_matrixUniformIndex,
		reinterpret_cast<const float*>(&mat)
	);

	_shaderProgram->setUniformVec2(_sizeUniformIndex, reinterpret_cast<const float*>(&_size));

	_shaderProgram->setUniformVec4(_colorUniformIndex, reinterpret_cast<const float*>(&_color));
}

void Widget::updateRecursively()
{
	update();
	for (Widget* widget : _children)
	{
		widget->updateRecursively();
	}
}

void Widget::update()
{
	const glm::vec2 parentPosition = _parent ? _parent->_transform.getPosition() : glm::vec2(0.0f, 0.0f);
	const glm::vec2 parentSize = _parent ? _parent->_size : (glm::vec2)Viewport::getCurrent()->getSize();
	const glm::vec2 halfParentSize = parentSize * 0.5f;

	switch (_anchor)
	{
	case Anchor::left:
	{
		_transform.setPosition(
			parentPosition -
			glm::vec2(halfParentSize.x, 0.0f) +
			glm::vec2(_margins.left, 0.0f) +
			_size * glm::vec2(0.5f, 0.0f)
		);
		break;
	}
	case Anchor::right:
	{
		_transform.setPosition(
			parentPosition +
			glm::vec2(halfParentSize.x, 0.0f) +
			glm::vec2(-_margins.right, 0.0f) +
			_size * glm::vec2(-0.5f, 0.0f) +
			_offset
		);
		break;
	}
	case Anchor::bottom:
	{
		_transform.setPosition(
			parentPosition +
			glm::vec2(0.0f, -halfParentSize.y) +
			glm::vec2(0.0f, _margins.bottom) +
			_size * glm::vec2(0.0f, 0.5f) +
			_offset
		);
		break;
	}
	case Anchor::top:
	{
		_transform.setPosition(
			parentPosition +
			glm::vec2(0.0f, halfParentSize.y) +
			glm::vec2(0.0f, -_margins.top) +
			_size * glm::vec2(0.0f, -0.5f) +
			_offset
		);
		break;
	}
	case Anchor::center:
	{
		_transform.setPosition(parentPosition + _offset);
		break;
	}
	case Anchor::leftBottom:
	{
		_transform.setPosition(
			parentPosition +
			glm::vec2(-halfParentSize.x, -halfParentSize.y) +
			glm::vec2(_margins.left, _margins.bottom) -
			_size * glm::vec2(0.5f, -0.5f) +
			_offset
		);
		break;
	}
	case Anchor::rightBottom:
	{
		_transform.setPosition(
			parentPosition +
			glm::vec2(halfParentSize.x, -halfParentSize.y) +
			glm::vec2(-_margins.right, _margins.bottom) -
			_size * glm::vec2(0.5f, -0.5f) +
			_offset
		);
		break;
	}
	case Anchor::topStretch:
	{
		_transform.setPosition(
			parentPosition +
			glm::vec2(0.0f, halfParentSize.y) +
			glm::vec2((_margins.left - _margins.right) * 0.5f, 0.0f) -
			glm::vec2(0.0f, _size.y * 0.5f) +
			_offset
		);
		_size.x = parentSize.x - _margins.left - _margins.right;
		break;
	}
	case Anchor::middleStretch:
	{
		_transform.setPosition(
			parentPosition +
			glm::vec2((_margins.left - _margins.right), (_margins.bottom - _margins.top)) * 0.5f +
			_offset
		);
		_size.x = parentSize.x - _margins.left - _margins.right;
		_size.y = parentSize.y - _margins.bottom - _margins.top;
		break;
	}
	default:
	{
		const uint8_t anchorLogValue = static_cast<uint8_t>(_anchor);
		std::cout << "WARNING::Widget: The anchor " << anchorLogValue << " is not implemented" << std::endl;
		break;
	}
	}
}
