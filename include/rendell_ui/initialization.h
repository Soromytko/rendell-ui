#pragma once
#include <functional>

namespace rendell_ui {
using InitRendellDelegate = std::function<void()>();

bool init(InitRendellDelegate initRendellDelegate = nullptr);
void release();
void draw();

void startFrame();
void endFrame();

void callInMainThread(std::function<void()> action);

} // namespace rendell_ui