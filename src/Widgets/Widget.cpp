#include <rendell_ui/Widgets/Widget.h>
#include <rendell_ui/Viewport.h>
#include <algorithm>

namespace rendell_ui
{
	static uint32_t s_instanceCount{ 0 };

	void widget_deleter(Widget* widgetPtr)
	{
		if (WidgetRegistrator::hasInstance())
		{
			WidgetRegistrator::getInstance()->unregisterWidget(widgetPtr);
		}
		delete widgetPtr;
	}

	void release_widget(const WidgetSharedPtr& widget)
	{
		// Remove this widget from parent's children
		// to remove from parent's ownership.
		widget->removeParent();
	}

	void initWidget(WidgetSharedPtr widget, WidgetWeakPtr parent)
	{
		widget->setSelfWeakPtr(widget);
		widget->setParent(parent);
		WidgetRegistrator::getInstance()->registerWidget(widget.get());
	}

	Widget::Widget() : IWidget()
	{

	}

	Widget::~Widget()
	{
		destroyed.emit();
	}

	void Widget::setVisible(bool value)
	{
		if (_visible != value)
		{
			_visible = value;
			updateImplicitVisibleRecursively();
			visibleChanged.emit(_visible);
		}
	}

	bool Widget::getVisible() const
	{
		return _visible;
	}

	bool Widget::getImplicitVisible() const
	{
		return _implicitVisible;
	}

	void Widget::setParent(WidgetWeakPtr widget)
	{
		WidgetSharedPtr lockedParent = _parent.lock();
		WidgetSharedPtr lockedWidget = widget.lock();

		if (lockedParent == lockedWidget || lockedWidget.get() == this)
		{
			return;
		}
		if (lockedParent)
		{
			lockedParent->_children.erase(_selfWeakPtr.lock());
		}
		if (lockedWidget)
		{
			lockedWidget->_children.insert(_selfWeakPtr.lock());
		}
		_parent = widget;

		updateImplicitVisibleRecursively();

		parentChanged.emit(_parent.lock());
	}

	void Widget::setSelfWeakPtr(WidgetWeakPtr value)
	{
		assert(value.lock().get() == this);
		_selfWeakPtr = value;
		onSelfWeakPtrChanged();
	}

	WidgetWeakPtr Widget::getParent() const
	{
		return _parent;
	}

	const std::unordered_set<WidgetSharedPtr>& Widget::getChildren() const
	{
		return _children;
	}

	const Transform2D& Widget::getTransform() const
	{
		return _transform;
	}

	void Widget::removeParent()
	{
		setParent({});
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

	bool Widget::intersect(glm::vec2 point) const
	{
		const glm::vec2 halfSize = _size * 0.5f * _transform.getScale();
		const glm::vec2 pos = _transform.getPosition();
		return std::clamp(point.x, pos.x - halfSize.x, pos.x + halfSize.x) == point.x &&
			std::clamp(point.y, pos.y - halfSize.y, pos.y + halfSize.y) == point.y;
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
		for (const WidgetSharedPtr& widget : _children)
		{
			widget->updateRecursively();
		}
	}

	void Widget::update()
	{
		WidgetSharedPtr parent = _parent.lock();
		const bool hasParent = parent != nullptr;

		const glm::vec2 parentPosition = hasParent ? parent->_transform.getPosition() : glm::vec2(0.0f, 0.0f);
		const glm::vec2 parentSize = hasParent ? parent->_size : (glm::vec2)Viewport::getCurrent()->getSize();
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
				glm::vec2(_margins.left, _margins.bottom) +
				_size * glm::vec2(0.5f, 0.5f) +
				_offset
			);
			break;
		}
		case Anchor::leftTop:
		{
			_transform.setPosition(
				parentPosition +
				glm::vec2(-halfParentSize.x, halfParentSize.y) +
				glm::vec2(_margins.left, -_margins.top) +
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
		case Anchor::rightTop:
		{
			_transform.setPosition(
				parentPosition +
				glm::vec2(halfParentSize.x, halfParentSize.y) +
				glm::vec2(-_margins.right, -_margins.top) +
				_size * glm::vec2(-0.5f, -0.5f) +
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
		case Anchor::horizontalStretch:
		{
			_transform.setPosition(
				parentPosition +
				glm::vec2((_margins.left - _margins.right) * 0.5f, 0.0f) +
				_offset
			);
			_size.x = parentSize.x - _margins.left - _margins.right;
			break;
		}
		case Anchor::bottomStretch:
		{
			_transform.setPosition(
				parentPosition +
				glm::vec2(0.0f, -halfParentSize.y) +
				glm::vec2((_margins.left - _margins.right) * 0.5f, _margins.bottom) +
				glm::vec2(0.0f, _size.y * 0.5f) +
				_offset
			);
			_size.x = parentSize.x - _margins.left - _margins.right;
			break;
		}
		case Anchor::leftStretch:
		{
			_transform.setPosition(
				parentPosition +
				glm::vec2(-halfParentSize.x, 0.0f) +
				glm::vec2(_margins.left, (_margins.bottom - _margins.top) * 0.5f) +
				glm::vec2(_size.x * 0.5f, 0.0f) +
				_offset
			);
			_size.y = parentSize.y - _margins.bottom - _margins.top;
			break;
		}
		case Anchor::verticalStretch:
		{
			_transform.setPosition(
				parentPosition +
				glm::vec2(0.0f, (_margins.bottom - _margins.top) * 0.5f) +
				_offset
			);
			_size.y = parentSize.y - _margins.bottom - _margins.top;
			break;
		}
		case Anchor::rightStretch:
		{
			_transform.setPosition(
				parentPosition +
				glm::vec2(halfParentSize.x, 0.0f) +
				glm::vec2(-_margins.right, (_margins.bottom - _margins.top) * 0.5f) +
				glm::vec2(-_size.x * 0.5f, 0.0f) +
				_offset
			);
			_size.y = parentSize.y - _margins.bottom - _margins.top;
			break;
		}
		case Anchor::centerStretch:
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

	void Widget::updateImplicitVisibleRecursively()
	{
		const WidgetSharedPtr& parent = _parent.lock();

		// Consider the absence of a parent as an implicit visibility.
		const bool parentImplicitVisible = parent != nullptr ? parent->_implicitVisible : true;

		const bool newImplicitVisible = parentImplicitVisible && _visible;
		if (_implicitVisible != newImplicitVisible)
		{
			_implicitVisible = newImplicitVisible;
			for (const WidgetSharedPtr& child : _children)
			{
				child->updateImplicitVisibleRecursively();
			}
		}
	}

}
