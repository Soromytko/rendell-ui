#include <ActionPool.h>

namespace rendell_ui {
size_t ActionPool::getActionCount() const {
    return _actions.size();
}

void ActionPool::addAction(Action action) {
    const std::lock_guard<std::mutex> lock(_mutex);
    _actions.push_back(action);
}

void ActionPool::execute() {
    std::vector<Action> actions;
    {
        const std::lock_guard<std::mutex> lock(_mutex);
        actions = std::move(_actions);
        _actions.clear();
    }

    for (const Action action : actions) {
        action();
    }
}

void ActionPool::clear() {
    const std::lock_guard<std::mutex> lock(_mutex);
    _actions.clear();
}
} // namespace rendell_ui
