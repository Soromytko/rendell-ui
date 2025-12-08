#include <FontStorage.h>

#include <rendell_text/IFontRaster.h>
#include <rendell_text/factory.h>
#include <rendell_ui/IFont.h>
#include <rendell_ui/font_loading.h>

#include "Font.h"
#include "logging.h"

#include <cassert>
#include <filesystem>

namespace rendell_ui {
static std::unique_ptr<FontStorage> s_instance{nullptr};

bool FontStorage::init() {
    assert(!s_instance);
    s_instance = std::make_unique<FontStorage>(new FontStorage);
    return true;
}

void FontStorage::release() {
    assert(s_instance);
    s_instance.reset();
}

FontStorage *FontStorage::getInstance() {
    assert(s_instance);
    return s_instance.get();
}

FontStorage::FontStorage() {
    const auto path = std::filesystem::path(FONT_ROOT_DIRECTORY) / "mononoki/mononoki-Regular.ttf";
    _defaultFont = loadFont(path);
    if (!_defaultFont) {
    }
}

static inline size_t getFontKey(const IFont &font) {
    return reinterpret_cast<size_t>(&font);
}

std::shared_ptr<IFont> FontStorage::getDefaultFont() const {
    return _defaultFont;
}

std::shared_ptr<rendell_text::IGlyphAtlasCache>
FontStorage::getOrCreateGlyphAtlasCache(const IFont &font) {
    const size_t key = getFontKey(font);
    auto it = _glyphAtlasCaches.find(key);
    if (it != _glyphAtlasCaches.end()) {
        if (auto lock = it->second.lock()) {
            return lock;
        }
    }

    auto fontRaster = rendell_text::createFontRaster();
    fontRaster->setFontData(font.getData(), font.getSize());
    auto glyphAtlasCache = rendell_text::createGlyphAtlasCache(fontRaster);
    _glyphAtlasCaches[key] = glyphAtlasCache;
    return glyphAtlasCache;
}

} // namespace rendell_ui