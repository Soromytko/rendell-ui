#pragma once
#include <memory>
#include <rendell_ui/Widgets/IWidget.h>
#include <rendell_ui/defines.h>

namespace rendell_ui {
class IWidgetCollection {
protected:
    IWidgetCollection() = default;

public:
    ~IWidgetCollection() = default;

    virtual void append(IWidget *value) = 0;
    virtual void remove(IWidget *value) = 0;
};

RENDELL_UI_DECLARE_SHARED_PTR(IWidgetCollection)
} // namespace rendell_ui