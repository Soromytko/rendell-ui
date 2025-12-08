#include <rendell_ui/font_loading.h>

#include "Font.h"

#include <cassert>
#include <logging.h>

namespace rendell_ui {
std::shared_ptr<IFont> loadFont(const std::filesystem::path &&path) {
    assert(!path.empty());

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        RUI_ERROR("Failed to open font file: {}", path.string());
        return nullptr;
    }

    std::streamsize size = file.tellg();
    if (size <= 0) {
        RUI_ERROR("Font file is empty: {}", path.string());
        return nullptr;
    }
    file.seekg(0, std::ios::beg);

    std::vector<std::byte> buffer(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char *>(buffer.data()), size)) {
        RUI_ERROR("Failed to read font file: {}", path.string());
        return nullptr;
    }

    return std::make_shared<Font>(std::move(buffer));
}
} // namespace rendell_ui