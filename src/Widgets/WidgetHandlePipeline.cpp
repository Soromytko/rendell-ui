#include "WidgetHandlePipeline.h"
#include <cassert>

namespace rendell_ui {
void WidgetHandlePipeline::append(IWidget *widget) {
    assert(widget);
    _widgets.insert(widget);
}

void WidgetHandlePipeline::remove(IWidget *widget) {
    assert(widget);
    _widgets.erase(widget);
}
} // namespace rendell_ui
