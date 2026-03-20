#pragma once
#include <cstdint>

namespace rendell_ui {
struct class VisibleTextBuffer final {
public:
    VisibleTextBuffer(size_t sizeInBytes);
    ~VisibleTextBuffer() = default;

    void setRect(uint32_t width, uint32_t height);
    void setOffset(double xOffset, double yOffset);

    //void updateLine(size_t size)

private:
};
} // namespace rendell_ui