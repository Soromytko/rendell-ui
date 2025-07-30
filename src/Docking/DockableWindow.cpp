#include <rendell_ui/Docking/DockableWindow.h>

namespace rendell_ui {
DockableWindow::DockableWindow(const std::string &title, WidgetSharedPtr canvasRootWidget)
    : IDockable()
    , _canvasRootWidget(canvasRootWidget) {
    _rootWidget = createDockRootWidget(_canvasRootWidget);
    _rootWidget->setAnchor(Anchor::center);
    setTitle(title);

    _rootWidget->getHeaderWidget()->headerDragged.connect([this]() {
        if (_headerDraggedCallback) {
            _headerDraggedCallback();
        }
    });
}

DockableWindow::~DockableWindow() {
    release_widget(_rootWidget);
}

glm::vec2 DockableWindow::getSize() const {
    return _size;
}

glm::vec2 DockableWindow::getMinSize() const {
    return _minSize;
}

glm::vec2 DockableWindow::getPosition() const {
    return _pos;
}

IDockable::MinSizeChangedCallback DockableWindow::getMinSizeChangedCallback() const {
    return _minSizeChangedCallback;
}

IDockable::HeaderDraggedCallback DockableWindow::getHeaderDraggedCallback() const {
    return _headerDraggedCallback;
}

void DockableWindow::setSize(glm::vec2 value) {
    if (_size != value) {
        _size = value;
        updateRootWidget();
    }
}

void DockableWindow::setMinSize(glm::vec2 value) {
    if (_minSize != value) {
        _minSize = value;
        if (_minSizeChangedCallback) {
            _minSizeChangedCallback(_minSize);
        }
    }
}

void DockableWindow::setPosition(glm::vec2 value) {
    if (_pos != value) {
        _pos = value;
        updateRootWidget();
    }
}

void DockableWindow::setMinSizeChangedCallback(MinSizeChangedCallback callback) {
    _minSizeChangedCallback = callback;
}

void DockableWindow::setHeaderDraggedCallback(HeaderDraggedCallback callback) {
    _headerDraggedCallback = callback;
}

const std::string &DockableWindow::getTitle() const {
    return _title;
}

DockRootWidgetSharedPtr DockableWindow::getRootWidget() const {
    return _rootWidget;
}

WidgetSharedPtr DockableWindow::getWorkSpaceWidget() const {
    return _rootWidget->getWorkSpaceWidget();
}

void DockableWindow::setTitle(const std::string &value) {
    if (_title != value) {
        _title = value;
        _rootWidget->getHeaderWidget()->setHeader(_title);
    }
}

void DockableWindow::updateRootWidget() {
    _rootWidget->setOffset(_pos);
    _rootWidget->setSize(_size);
}

} // namespace rendell_ui