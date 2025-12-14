#pragma once
#include <cstdint>

namespace rendell_ui {
class ITextModel {
public:
    ITextModel() = default;
    virtual ~ITextModel() = default;

    virtual uint32_t getHeight() const = 0;
};
} // namespace rendell_ui