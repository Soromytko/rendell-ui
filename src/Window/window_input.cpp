#include <GLFW/glfw3.h>
#include <rendell_ui/Window/window_input.h>

namespace rendell_ui {
static int convertCustomModToGlfwMod(InputMod mod) {
    switch (mod) {
    case InputMod::alt:
        return GLFW_MOD_ALT;
    case InputMod::ctrl:
        return GLFW_MOD_CONTROL;
    }

    return 0;
}

InputKey convertGlfwKeyToInputKey(int key) {
    switch (key) {
    case GLFW_KEY_SPACE:
        return InputKey::space;
    case GLFW_KEY_A:
        return InputKey::A;
    case GLFW_KEY_B:
        return InputKey::B;
    case GLFW_KEY_C:
        return InputKey::C;
    case GLFW_KEY_D:
        return InputKey::D;
    case GLFW_KEY_E:
        return InputKey::E;
    case GLFW_KEY_F:
        return InputKey::F;
    case GLFW_KEY_G:
        return InputKey::G;
    case GLFW_KEY_H:
        return InputKey::H;
    case GLFW_KEY_I:
        return InputKey::I;
    case GLFW_KEY_J:
        return InputKey::J;
    case GLFW_KEY_K:
        return InputKey::K;
    case GLFW_KEY_L:
        return InputKey::L;
    case GLFW_KEY_M:
        return InputKey::M;
    case GLFW_KEY_N:
        return InputKey::N;
    case GLFW_KEY_O:
        return InputKey::O;
    case GLFW_KEY_P:
        return InputKey::P;
    case GLFW_KEY_Q:
        return InputKey::Q;
    case GLFW_KEY_R:
        return InputKey::R;
    case GLFW_KEY_S:
        return InputKey::S;
    case GLFW_KEY_T:
        return InputKey::T;
    case GLFW_KEY_U:
        return InputKey::U;
    case GLFW_KEY_V:
        return InputKey::V;
    case GLFW_KEY_W:
        return InputKey::W;
    case GLFW_KEY_X:
        return InputKey::X;
    case GLFW_KEY_Y:
        return InputKey::Y;
    case GLFW_KEY_Z:
        return InputKey::Z;
    case GLFW_KEY_ENTER:
        return InputKey::enter;
    case GLFW_KEY_TAB:
        return InputKey::tab;
    case GLFW_KEY_BACKSPACE:
        return InputKey::backspace;
    case GLFW_KEY_DELETE:
        return InputKey::del;
    case GLFW_KEY_RIGHT:
        return InputKey::right;
    case GLFW_KEY_LEFT:
        return InputKey::left;
    case GLFW_KEY_DOWN:
        return InputKey::down;
    case GLFW_KEY_UP:
        return InputKey::up;
    }
    return InputKey::none;
}

InputMouseButton convertGlfwMouseButtonToInputMouseButton(int button) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        return InputMouseButton::leftButton;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        return InputMouseButton::middleButton;
    case GLFW_MOUSE_BUTTON_RIGHT:
        return InputMouseButton::rightButton;
    }
    return InputMouseButton::none;
}

InputAction convertGlfwActionToInputAction(int action) {
    switch (action) {
    case GLFW_PRESS:
        return InputAction::pressed;
    case GLFW_RELEASE:
        return InputAction::release;
    case GLFW_REPEAT:
        return InputAction::repeat;
    }
    return InputAction::none;
}

InputModControl::InputModControl(int mods)
    : _mods(mods) {
}

bool InputModControl::hasMod(InputMod mod, int mods) const {
    return mods & convertCustomModToGlfwMod(mod);
}

bool InputModControl::hasModOnly(InputMod mod, int mods) const {
    return mods == convertCustomModToGlfwMod(mod);
}

bool InputModControl::hasCtrlMod() const {
    return _mods & GLFW_MOD_CONTROL;
}

bool InputModControl::hasAltMod() const {
    return _mods & GLFW_MOD_ALT;
}

bool InputModControl::hasShiftMod() const {
    return _mods & GLFW_MOD_SHIFT;
}
} // namespace rendell_ui
