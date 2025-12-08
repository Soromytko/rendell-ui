#include <Font.h>

#include <cassert>

namespace rendell_ui {
Font::Font(std::vector<std::byte> &&data)
    : _dataContainer(std::move(data)) {
    assert(_dataContainer.size() > 0);
}

const std::byte *Font::getData() const {
    return _dataContainer.data();
}

const size_t Font::getSize() const {
    return _dataContainer.size();
}
} // namespace rendell_ui