#include "IDockable.h"
#include <rendell_ui/Docking/Widgets/DockRootWidget.h>
#include <rendell_ui/Widgets/Widget.h>
#include <string>

namespace rendell_ui {
class DockableWindow : public IDockable {
public:
    DockableWindow(const std::string &title, WidgetSharedPtr canvasRootWidget);
    virtual ~DockableWindow();

    // IDockable
    glm::vec2 getSize() const override;
    glm::vec2 getMinSize() const override;
    glm::vec2 getPosition() const override;
    MinSizeChangedCallback getMinSizeChangedCallback() const override;
    virtual HeaderDraggedCallback getHeaderDraggedCallback() const override;

    // IDockable
    void setSize(glm::vec2 value) override;
    void setMinSize(glm::vec2 value) override;
    void setPosition(glm::vec2 value) override;
    void setMinSizeChangedCallback(MinSizeChangedCallback callback) override;
    virtual void setHeaderDraggedCallback(HeaderDraggedCallback callback) override;

    const std::string &getTitle() const;
    DockRootWidgetSharedPtr getRootWidget() const;
    WidgetSharedPtr getWorkSpaceWidget() const;

    void setTitle(const std::string &value);

private:
    void updateRootWidget();

    glm::vec2 _size{100.0f, 100.0f};
    glm::vec2 _minSize{100.0f, 100.0f};
    glm::vec2 _pos{100.f, 100.0f};

    std::string _title{};
    DockRootWidgetSharedPtr _rootWidget;
    WidgetSharedPtr _canvasRootWidget;
    MinSizeChangedCallback _minSizeChangedCallback;
    HeaderDraggedCallback _headerDraggedCallback;
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(DockableWindow)

} // namespace rendell_ui