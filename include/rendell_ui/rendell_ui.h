
#pragma once
#include "Canvas.h"
#include "Viewport.h"
#include "color_defines.h"
#include "defines.h"
#include "initialization.h"

#include "Window/IWindow.h"
#include "Window/window_input.h"

#include "Widgets/ButtonWidget.h"
#include "Widgets/PageViewerWidget.h"
#include "Widgets/RectangleWidget.h"
#include "Widgets/TabBarWidget.h"
#include "Widgets/TabButtonWidget.h"
#include "Widgets/TextEditWidget.h"
#include "Widgets/TextWidget.h"

#include "Docking/DockCanvas.h"
#include "Docking/DockNode.h"
#include "Docking/DockTree.h"
#include "Docking/DockTreeBuilder.h"
#include "Docking/DockableWindow.h"
#include "Docking/IDockable.h"
#include "Docking/Widgets/DockHeaderWidget.h"
#include "Docking/Widgets/DockRootWidget.h"
#include "Docking/Widgets/DockSeparatorWidget.h"

namespace rendell_ui {
IWindow *createWindow(uint32_t width, uint32_t height, const char *title, bool isHidden = false);
void releaseWindow(IWindow *window);
size_t getWindowCount();
std::vector<IWindow *> getWindows();
bool isValidWindow(IWindow *window);
} // namespace rendell_ui
