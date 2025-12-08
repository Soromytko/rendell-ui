#include <rendell_ui/initialization.h>

#include "Shaders/ShaderStorage.h"
#include "Time.h"
#include "Widgets/WidgetHandlePipeline.h"
#include "Widgets/WidgetRegistrator.h"
#include "Widgets/WidgetRenderPipeline.h"
#include "Window/WindowManager.h"
#include <ActionPool.h>
#include <FontStorage.h>
#include <GLFW/glfw3.h>
#include <logging.h>
#include <rendell/init.h>

namespace rendell_ui {
static WidgetRenderPipelineSharedPtr s_widgetRenderPipeline{nullptr};
static ActionPoolSharedPtr s_actionPool{nullptr};

bool init(InitRendellDelegate initRendellDelegate) {
    if (initRendellDelegate) {
        initRendellDelegate();
    } else {
        rendell::init(rendell::Initer());
    }
    FontStorage::init();
    WindowManager::init();
    WindowManager::getInstance()->reserveWindow();

    s_widgetRenderPipeline = makeWidgetRenderPipeline();

    WidgetRegistrator::init({
        makeWidgetHandlePipeline(),
        s_widgetRenderPipeline,
    });

    ShaderStorage::init();

    s_actionPool = makeActionPool();

    return true;
}

void release() {
    FontStorage::release();
    WidgetRegistrator::release();
    ShaderStorage::release();
    WindowManager::release();
    rendell::release();
    s_actionPool = nullptr;
}

void draw() {
    Time::updateDeltaTime();
    s_widgetRenderPipeline->draw();
}

void startFrame() {
    s_actionPool->execute();
}

void endFrame() {
}

void callInMainThread(std::function<void()> action) {
    s_actionPool->addAction(action);
    glfwPostEmptyEvent();
}

} // namespace rendell_ui
