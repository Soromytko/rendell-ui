#pragma once
#include <rendell_ui/IFont.h>

#include <vector>

namespace rendell_ui {
class Font final : public IFont {
public:
    Font(std::vector<std::byte> &&data);
    ~Font() = default;

    const std::byte *getData() const override;
    const size_t getSize() const override;

private:
    std::vector<std::byte> _dataContainer;
};
} // namespace rendell_ui