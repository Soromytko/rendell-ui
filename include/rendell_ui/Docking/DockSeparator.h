#pragma once

namespace rendell_ui {
class DockSeparator final {
public:
    enum class Type {
        horizontal,
        vertical,
    };

    DockSeparator() = default;
    ~DockSeparator() = default;

    // Type getType() const;

private:
    Type _type{Type::horizontal};
};
} // namespace rendell_ui