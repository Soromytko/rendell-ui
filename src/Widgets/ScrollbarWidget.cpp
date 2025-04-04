#include <rendell_ui/Widgets/ScrollbarWidget.h>
#include <algorithm>

#define DEFAULT_WIDTH 20

namespace rendell_ui
{
	ScrollbarWidget::ScrollbarWidget()
	{
		setSize({ DEFAULT_WIDTH, 0.0f });

		_backgroundRectangleWidget = createUnregisteredRectangleWidget();
		_backgroundRectangleWidget->setInteract(false);
		_backgroundRectangleWidget->setAnchor(Anchor::centerStretch);
		_backgroundRectangleWidget->setColor({ 0.2f, 0.2f, 0.2f, 1.0f });

		_handleRectangleWidget = createUnregisteredRectangleWidget(_backgroundRectangleWidget);
		_handleRectangleWidget->setInteract(false);
		_handleRectangleWidget->setAnchor(Anchor::topStretch);
		_handleRectangleWidget->setColor({ 1.0f, 0.0f, 0.0f, 1.0f });

		sizeChanged.connect([&](glm::vec2 size) { updateHandle(); });
	}

	void ScrollbarWidget::draw() const
	{
		if (_isHidden)
		{
			return;
		}
		_backgroundRectangleWidget->draw();
		_handleRectangleWidget->draw();
	}

	void ScrollbarWidget::onMouseDown(glm::dvec2 cursorPosition)
	{
		processMouseInput(cursorPosition);
	}

	void ScrollbarWidget::onDragged(glm::dvec2 startPoint, glm::dvec2 endPoint)
	{
		processMouseInput(endPoint);
	}

	float ScrollbarWidget::getProgress() const
	{
		return _progress;
	}

	IScrollableWidgetWeakPtr ScrollbarWidget::getScrollable() const
	{
		return _scrollable;
	}

	void ScrollbarWidget::updateProgress()
	{
		_progress = _scrollable.expired() ? 0.0f : _scrollable.lock()->getScrollProgress();
		updateHandle();
	}

	void ScrollbarWidget::setScrollable(IScrollableWidgetWeakPtr scrollable)
	{
		if (_scrollable.owner_before(scrollable) || scrollable.owner_before(_scrollable))
		{
			_scrollable = std::move(scrollable);
			updateHandle();
		}
	}

	void ScrollbarWidget::setProgress(float value)
	{
		value = std::clamp(value, 0.0f, 1.0f);
		if (_progress != value)
		{
			_progress = value;
			updateHandle();
		}
	}

	void ScrollbarWidget::onSelfWeakPtrChanged()
	{
		_backgroundRectangleWidget->setParent(_selfWeakPtr);
		_backgroundRectangleWidget->updateRecursively();
	}

	void ScrollbarWidget::processMouseInput(glm::dvec2 cursorPosition)
	{
		const glm::dvec2 localPosition = cursorPosition - static_cast<glm::dvec2>(_transform.getPosition());
		const glm::dvec2 offsetLocalPosition = -(localPosition + glm::dvec2(0.5f, -0.5f) * static_cast<glm::dvec2>(_size));
		const glm::vec2 halfHandleSize = _handleRectangleWidget->getSize() * 0.5f;
		const float progress = (offsetLocalPosition.y - halfHandleSize.y) / (_size.y - halfHandleSize.y * 2);
		setProgress(progress);
		if (auto locked = _scrollable.lock())
		{
			locked->setScrollProgress(_progress);
		}
	}

	void ScrollbarWidget::updateHandle()
	{
		const float handleHeight = calculateHandleHeight();
		const float offset = -(_size.y - handleHeight) * _progress;
		_handleRectangleWidget->setSize({ _size.x, handleHeight });
		_handleRectangleWidget->setOffset({ 0.0f, offset });
		_isHidden = handleHeight >= _size.y;
	}

	float ScrollbarWidget::calculateHandleHeight()
	{
		if (auto locked = _scrollable.lock())
		{
			return locked->getScrollRatio() * _size.y;
		}
		return _size.y;
	}


}