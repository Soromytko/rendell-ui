#pragma once
#include <cstddef>

namespace rendell_ui {
class IFont {
public:
    IFont() = default;
    virtual ~IFont() = default;

    virtual const std::byte *getData() const = 0;
    virtual const size_t getSize() const = 0;
};
} // namespace rendell_ui