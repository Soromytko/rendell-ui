#pragma once
#include <memory>
#include <unordered_map>

namespace rendell_text {
class IGlyphAtlasCache;
} // namespace rendell_text

namespace rendell_ui {
class IFont;

class FontStorage final {
public:
    static bool init();
    static void release();
    static FontStorage *getInstance();

private:
    FontStorage();

public:
    ~FontStorage() = default;

    std::shared_ptr<IFont> getDefaultFont() const;

    std::shared_ptr<rendell_text::IGlyphAtlasCache> getOrCreateGlyphAtlasCache(const IFont &font);

private:
    std::shared_ptr<IFont> _defaultFont;
    std::unordered_map<size_t, std::weak_ptr<rendell_text::IGlyphAtlasCache>> _glyphAtlasCaches{};
};
} // namespace rendell_ui