#pragma once
#include "IWidgetCollection.h"
#include <set>

namespace rendell_ui {
class WidgetHandlePipeline : public IWidgetCollection {
public:
    WidgetHandlePipeline() = default;
    ~WidgetHandlePipeline() = default;

    void append(IWidget *widget) override;
    void remove(IWidget *widget) override;

private:
    std::set<IWidget *> _widgets{};
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(WidgetHandlePipeline)
} // namespace rendell_ui
