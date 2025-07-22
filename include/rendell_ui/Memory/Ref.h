#pragma once
#include <memory>
#include <vector>

namespace rendell_ui {
template <typename T> using Ref = std::shared_ptr<T>;
template <typename T> using WeakRef = std::weak_ptr<T>;
template <typename T> using RefList = std::vector<Ref<T>>;

template <typename T, typename... Args> Ref<T> make_ref(Args &&...args) {
    return Ref<T>(std::make_shared<T>(std::forward<Args>(args)...));
}
} // namespace rendell_ui