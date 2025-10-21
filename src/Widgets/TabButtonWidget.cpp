#include <glm/gtc/matrix_transform.hpp>
#include <rendell_ui/Viewport.h>
#include <rendell_ui/Widgets/TabButtonWidget.h>

#define SELECTED_COLOR                                                                             \
    { 0.8f, 0.0f, 0.0f, 1.0f }
#define SELECTED_HOVERED_COLOR                                                                     \
    { 1.0f, 0.0f, 0.0f, 1.0f }
#define UNSELECTED_COLOR                                                                           \
    { 0.0f, 0.0f, 0.0f, 1.0f }
#define UNSELECTED_HOVERED_COLOR                                                                   \
    { 0.3f, 0.0f, 0.0f, 1.0f }

namespace rendell_ui {
TabButtonWidget::TabButtonWidget(std::wstring title)
    : _title(std::move(title)) {
    _color = UNSELECTED_COLOR;

    _rectangle = createUnregisteredRectangleWidget();
    _rectangle->setAnchor(Anchor::centerStretch);
    _rectangle->setInteract(false);
    _rectangle->setColor(_color);

    _textWidget = createUnregisteredTextWidget();
    _textWidget->setAnchor(Anchor::centerStretch);
    _textWidget->setInteract(false);
    _textWidget->setBackgroundColor(_color);
    _textWidget->setText(_title);
    _textWidget->setColor({0.0f, 1.0f, 0.0f, 1.0f});
}

bool TabButtonWidget::getIsSelected() const {
    return _isSelected;
}

const std::wstring &TabButtonWidget::getTitle() const {
    return _title;
}

void TabButtonWidget::draw() {
    _rectangle->draw();
    _textWidget->draw();
}

void TabButtonWidget::setTitle(std::wstring value) {
    _title = std::move(value);
    if (_textWidget) {
        _textWidget->setText(_title);
    }
}

void TabButtonWidget::setIsSelected(bool value) {
    if (_isSelected != value) {
        _isSelected = value;
        onIsSelectedChanged();
        isSelectedChanged.emit(this);
    }
}

void TabButtonWidget::onMouseEntered() {
    if (_isSelected) {
        setColor(SELECTED_HOVERED_COLOR);
    } else {
        setColor(UNSELECTED_HOVERED_COLOR);
    }
}

void TabButtonWidget::onMouseExited() {
    if (_isSelected) {
        setColor(SELECTED_COLOR);
    } else {
        setColor(UNSELECTED_COLOR);
    }
}

void TabButtonWidget::onIsSelectedChanged() {
    setColor(_isSelected ? glm::vec4(SELECTED_COLOR) : glm::vec4(UNSELECTED_COLOR));
}

void TabButtonWidget::onSelfWeakPtrChanged() {
    if (_rectangle) {
        _rectangle->setParent(_selfWeakPtr);
    }

    if (_textWidget) {
        _textWidget->setParent(_selfWeakPtr);
    }
}

void TabButtonWidget::onColorChanged() {
    if (_rectangle) {
        _rectangle->setColor(_color);
    }

    if (_textWidget) {
        _textWidget->setBackgroundColor(_color);
    }
}

void TabButtonWidget::onMouseDown(glm::dvec2 cursorPosition) {
    setIsSelected(true);
    setColor(SELECTED_HOVERED_COLOR);
}

} // namespace rendell_ui
