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

	void initWidget(WidgetSharedPtr widget, WidgetWeakPtr parent, bool registered)
	{
		widget->setSelfWeakPtr(widget);
		widget->setParent(parent);
		if (registered)
		{
			WidgetRegistrator::getInstance()->registerWidget(widget.get());
		}
	}

	Widget::Widget() : AnchorableWidget()
	{

	}

	Widget::~Widget()
	{
		destroyed.emit();
	}

	void Widget::onMarkupUpdated()
	{
		for (const WidgetSharedPtr& child : _children)
		{
			child->updateMarkup();
		}
	}

	glm::vec2 Widget::getRootPosition() const
	{
		if (const WidgetSharedPtr maybeParent = _parent.lock())
		{
			return maybeParent->_transform.getPosition();
		}
		return glm::vec2(0.0f, 0.0f);
	}

	glm::vec2 Widget::getRootSize() const
	{
		if (const WidgetSharedPtr maybeParent = _parent.lock())
		{
			return maybeParent->_size;
		}
		return static_cast<glm::vec2>(Viewport::getCurrent()->getSize());
	}

	void Widget::setVisible(bool value)
	{
		if (_visible != value)
		{
			_visible = value;
			onVisibleChanged();
			updateImplicitVisibleRecursively();
			visibleChanged.emit(_visible);
		}
	}

	bool Widget::getVisible() const
	{
		return _visible;
	}

	void Widget::setInteract(bool value)
	{
		if (_interact != value)
		{
			_interact = value;
			onInteractChanged();
		}
	}

	bool Widget::getInteract() const
	{
		return _interact;
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
			lockedParent->removeChild(_selfWeakPtr.lock());
		}
		if (lockedWidget)
		{
			lockedWidget->addChild(_selfWeakPtr.lock());
		}
		_parent = widget;
		onParentChanged();
		parentChanged.emit(_parent.lock());
		updateRecursively();

		updateImplicitVisibleRecursively();
	}

	void Widget::setSelfWeakPtr(WidgetWeakPtr value)
	{
		assert(value.lock().get() == this);
		_selfWeakPtr = value;
		onSelfWeakPtrChanged();
	}

	void Widget::addChild(WidgetSharedPtr child)
	{
		if (_children.find(child) == _children.end())
		{
			_children.insert(child);
			onChildrenChanged();
			childrenChanged.emit();
		}
	}

	void Widget::removeChild(WidgetSharedPtr child)
	{
		if (auto it = _children.find(child); it != _children.end())
		{
			_children.erase(it);
			onChildrenChanged();
			childrenChanged.emit();
		}
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

	void Widget::setName(const std::string& name)
	{
		if (_name != name)
		{
			_name = name;
			onNameChanged();
			nameChanged.emit(_name);
		}
	}

	const std::string& Widget::getName() const
	{
		return _name;
	}

	void Widget::setColor(glm::vec4 value)
	{
		if (_color != value)
		{
			_color = value;
			onColorChanged();
			colorChanged.emit(_color);
		}
	}

	glm::vec4 Widget::getColor() const
	{
		return _color;
	}

	void Widget::updateRecursively()
	{
		updateMarkup();
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
