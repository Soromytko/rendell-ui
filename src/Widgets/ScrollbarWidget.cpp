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

	void ScrollbarWidget::onCaptured(glm::dvec2 cursorPosition)
	{
		if (!_handleRectangleWidget->intersect(cursorPosition))
		{
			const glm::dvec2 localPosition = cursorPosition - static_cast<glm::dvec2>(_transform.getPosition());
			const glm::dvec2 offsetLocalPosition = -(localPosition + glm::dvec2(0.5, -0.5) * static_cast<glm::dvec2>(_size));
			const glm::dvec2 halfHandleSize = static_cast<glm::dvec2>(_handleRectangleWidget->getSize()) * 0.5;
			const double progress = (offsetLocalPosition.y - halfHandleSize.y) / (_size.y - halfHandleSize.y * 2.0);
			setProgress(progress);
			if (auto locked = _scrollable.lock())
			{
				locked->setScrollProgress(_progress);
			}
		}
		startDragging();
	}

	void ScrollbarWidget::onDragged(glm::dvec2 startPoint, glm::dvec2 endPoint)
	{
		processDragging(startPoint - endPoint);
	}

	void ScrollbarWidget::onMouseScrolled(glm::dvec2 scroll)
	{
		if (auto locked = _scrollable.lock())
		{
			locked->onProcessMouseScrolled(scroll);
		}
	}

	double ScrollbarWidget::getProgress() const
	{
		return _progress;
	}

	IScrollableWidgetWeakPtr ScrollbarWidget::getScrollable() const
	{
		return _scrollable;
	}

	void ScrollbarWidget::updateProgress()
	{
		_progress = _scrollable.expired() ? 0.0 : _scrollable.lock()->getScrollProgress();
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

	void ScrollbarWidget::setProgress(double value)
	{
		value = std::clamp(value, 0.0, 1.0);
		if (_progress != value)
		{
			_progress = value;
			updateHandle();
		}
	}

	void ScrollbarWidget::onSelfWeakPtrChanged()
	{
		_backgroundRectangleWidget->setParent(_selfWeakPtr);
	}

	void ScrollbarWidget::startDragging()
	{
		_startDraggingHandleOffset = _handleRectangleWidget->getOffset();
	}

	void ScrollbarWidget::processDragging(glm::dvec2 dragOffset)
	{
		double offset = (_startDraggingHandleOffset.y - dragOffset.y);
		offset = std::clamp(offset, static_cast<double>( - _size.y + _handleRectangleWidget->getSize().y), 0.0);
		const double progress = std::abs(offset / (_size.y - _handleRectangleWidget->getSize().y));

		setProgress(progress);
		if (auto locked = _scrollable.lock())
		{
			locked->setScrollProgress(_progress);
		}
	}

	void ScrollbarWidget::updateHandle()
	{
		const float handleHeight = calculateHandleHeight();
		const float offset = -(_size.y - handleHeight) * static_cast<float>(_progress);
		_handleRectangleWidget->setSize({ _size.x, handleHeight });
		_handleRectangleWidget->setOffset({ 0.0f, offset });
		_isHidden = handleHeight >= _size.y;
	}

	float ScrollbarWidget::calculateHandleHeight()
	{
		if (auto locked = _scrollable.lock())
		{
			return static_cast<float>(locked->getScrollRatio()) * _size.y;
		}
		return _size.y;
	}


}