#include "Time.h"
#include <GLFW/glfw3.h>

namespace rendell_ui {
static double s_deltaTime = 0.0;
static double s_previousTime = 0.0;

void Time::updateDeltaTime() {
    const double currentTime = getCurrentTime();
    s_deltaTime = currentTime - s_previousTime;
    s_previousTime = currentTime;
}

double Time::getDeltaTime() {
    return s_deltaTime;
}

double Time::getCurrentTime() {
    return glfwGetTime();
}
} // namespace rendell_ui