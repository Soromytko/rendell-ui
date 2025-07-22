#pragma once
#include "IWidgetCollection.h"
#include <memory>
#include <rendell_ui/defines.h>
#include <vector>

namespace rendell_ui {
class WidgetRenderPipeline : public IWidgetCollection {
public:
    WidgetRenderPipeline() = default;
    ~WidgetRenderPipeline() = default;

    void append(IWidget *widget) override;
    void remove(IWidget *widget) override;

    void draw() const;

private:
    std::vector<IWidget *> _widgets{};
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(WidgetRenderPipeline)
} // namespace rendell_ui
