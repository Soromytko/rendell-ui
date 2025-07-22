#pragma once
#include <functional>

namespace rendell_ui {
bool init();
void release();
void draw();

void startFrame();
void endFrame();

void callInMainThread(std::function<void()> action);

} // namespace rendell_ui