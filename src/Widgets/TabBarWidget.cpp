#include <rendell_ui/Widgets/TabBarWidget.h>
#include <rendell_ui/Widgets/RectangleWidget.h>
#include <logging.h>
#include <algorithm>

#define TAB_BAR_HEIGHT 30.0f
#define TAB_BUTTON_WIDTH 200.0f
#define TAB_BUTTON_SPACING 2.0f

namespace rendell_ui
{
	static TabBarWidget::CreateTabButtonDelegate s_defaultCreateTabButtonDelegate = [](std::wstring title)
		{
			const auto button = createUnregisteredTabButtonWidget();
			button->setTitle(std::move(title));
			button->setAnchor(Anchor::leftStretch);
			button->setSize({ TAB_BUTTON_WIDTH, 0.0f });
			return button;
		};

	TabBarWidget::TabBarWidget(std::initializer_list<StringListModel::String> tabNames) :
		_createTabButtonDelegate(s_defaultCreateTabButtonDelegate)
	{
		setAnchor(Anchor::topStretch);
		setSize({ 0.0f, TAB_BAR_HEIGHT });

		const auto model = makeStringListModel(tabNames);
		setModel(model);
	}

	StringListModelSharedPtr TabBarWidget::getModel() const
	{
		return _model;
	}

	TabBarWidget::CreateTabButtonDelegate TabBarWidget::getCreateTabButtonDelegate() const
	{
		return _createTabButtonDelegate;
	}

	void TabBarWidget::draw() const
	{
		for (const TabButtonWidgetSharedPtr& button : _buttons)
		{
			button->draw();
		}
	}

	void TabBarWidget::setModel(StringListModelSharedPtr model)
	{
		if (!model)
		{
			RUI_ERROR << "The Model is null";
			return;
		}

		if (_model != model)
		{
			if (_model)
			{
				_model->itemAdded.disconnect(_itemAddedConnectionId);
				_model->itemRemoved.disconnect(_itemRemovedConnectionId);
				_model->itemChanged.disconnect(_itemChangedConnectionId);
			}

			_model = model;

			if (_model)
			{
				_itemAddedConnectionId = _model->itemAdded.connect(this, &TabBarWidget::onModelItemAdded);
				_itemRemovedConnectionId = _model->itemRemoved.connect(this, &TabBarWidget::onModelItemRemoved);
				_itemChangedConnectionId = _model->itemChanged.connect(this, &TabBarWidget::onModelItemChanged);
			}

			updateButtons();
			updateButtonMarkup();
			setCurrentIndex(-1);
		}
	}

	void TabBarWidget::setCreateTabButtonDelegate(CreateTabButtonDelegate delegate)
	{
		_createTabButtonDelegate = delegate;
		_buttons.clear();
		updateButtons();
	}

	bool TabBarWidget::setCurrentIndex(int index)
	{
		if (_currentIndex != index)
		{
			_currentIndex = index;
			updateButtonSelection();
			currentIndexChanged.emit(_currentIndex);
			return true;
		}
		return false;
	}

	void TabBarWidget::resetCreateTabButtonDelegate()
	{
		setCreateTabButtonDelegate(s_defaultCreateTabButtonDelegate);
	}

	void TabBarWidget::onSelfWeakPtrChanged()
	{
		for (const TabButtonWidgetSharedPtr button : _buttons)
		{
			button->setParent(_selfWeakPtr);
		}
	}

	void TabBarWidget::onMarkupUpdated()
	{
		Widget::onMarkupUpdated();
		updateButtonMarkup();
	}

	void TabBarWidget::updateButtonSelection()
	{
		const bool isOkIndex = _currentIndex >= 0 && _currentIndex < _buttons.size();
		for (size_t i = 0; i < _buttons.size(); i++)
		{
			_buttons[i]->setIsSelected(isOkIndex && static_cast<int>(i) == _currentIndex);
		}
	}

	void TabBarWidget::selectButton(TabButtonWidget* button)
	{
		auto it = std::find_if(_buttons.begin(), _buttons.end(),
			[=](const TabButtonWidgetSharedPtr& currentButton) { return currentButton.get() == button; });

		if (it == _buttons.end())
		{
			RUI_ERROR << "Invalid button index; _buttons.size() = " << _buttons.size();
			return;
		}

		const size_t index = std::distance(_buttons.begin(), it);
		setCurrentIndex(static_cast<int>(index));
	}

	void TabBarWidget::onModelItemAdded(const std::wstring& item, size_t itemIndex)
	{
		const TabButtonWidgetSharedPtr button = createTabButton(item);
		_buttons.insert(_buttons.begin() + itemIndex, button);

		if (_currentIndex > itemIndex)
		{
			setCurrentIndex(_currentIndex + 1);
		}
		else if (_currentIndex == itemIndex)
		{
			_currentIndex = -1;
			setCurrentIndex(static_cast<int>(itemIndex));
		}

		updateButtonMarkup();
	}

	void TabBarWidget::onModelItemRemoved(const std::wstring& item, size_t itemIndex)
	{
		_buttons.erase(_buttons.begin() + itemIndex);
		if (_currentIndex >= itemIndex)
		{
			if (_buttons.size() == 0)
			{
				setCurrentIndex(-1);
			}
			else
			{
				const int newIndex = std::clamp(_currentIndex + 1, 0, static_cast<int>(_buttons.size()) - 1);
				if (!setCurrentIndex(newIndex))
				{
					updateButtonSelection();
				}
			}
		}
		updateButtonMarkup();
	}

	void TabBarWidget::onModelItemChanged(const std::wstring& item, size_t itemIndex)
	{
		_buttons[itemIndex]->setTitle(item);
	}

	void TabBarWidget::onTabButtonIsSelectedChanged(TabButtonWidget* button)
	{
		if (button->getIsSelected())
		{
			selectButton(button);
		}
	}

	int TabBarWidget::getButtonIndexUnderCursor(glm::dvec2 cursorPosition) const
	{
		for (size_t i = 0; i < _buttons.size(); i++)
		{
			if (_buttons[i]->intersect(cursorPosition))
			{
				return static_cast<int>(i);
			}
		}
		return -1;
	}

	TabButtonWidgetSharedPtr TabBarWidget::createTabButton(std::wstring title)
	{
		const auto button = _createTabButtonDelegate(std::move(title));
		button->setParent(_selfWeakPtr);
		// We assume that the button will be removed completely,
		// so we do not unsubscribe from the signal
		button->isSelectedChanged.connect(this, &TabBarWidget::onTabButtonIsSelectedChanged);
		return button;
	}

	void TabBarWidget::updateButtons()
	{
		if (!_model)
		{
			_buttons.clear();
			return;
		}

		_buttons.resize(_model->getSize());

		for (size_t i = 0; i < _buttons.size(); i++)
		{
			auto title = _model->operator[](i);
			if (!_buttons[i])
			{
				_buttons[i] = createTabButton(std::move(title));
			}
			else
			{
				_buttons[i]->setTitle(std::move(title));
			}
		}
	}

	void TabBarWidget::updateButtonMarkup()
	{
		float horizontalOffset{};
		for (const TabButtonWidgetSharedPtr button : _buttons)
		{
			button->setOffset({ horizontalOffset, 0.0f });
			button->setParent(_selfWeakPtr);
			button->updateRecursively();
			horizontalOffset += button->getSize().x + TAB_BUTTON_SPACING;
		}
	}

}
