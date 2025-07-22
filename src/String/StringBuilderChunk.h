#pragma once
#include <memory>
#include <rendell_ui/defines.h>
#include <string>
#include <vector>

namespace rendell_ui {
typedef std::wstring StringType;

class StringBuilderChunk final {
public:
    StringBuilderChunk(size_t size = 16, StringType &&data = {});
    ~StringBuilderChunk() = default;

    bool isFull() const;
    size_t getCurrentLength() const;
    size_t getRemainingLength() const;
    const StringType &getData() const;
    StringType toString() const;

    StringType getSubData(size_t indexFrom) const;
    StringType getSubData(size_t indexFrom, size_t count) const;

    size_t append(const StringType &data, size_t startIndex = 0);
    size_t append(const StringBuilderChunk &chunk);
    size_t insert(size_t index, const StringType &data, size_t dataOffsetIndex = 0);

    void removeAt(size_t index);
    void removeEnd(size_t indexFrom);
    void remove(size_t indexFrom, size_t count);

protected:
    size_t _currentLength{0};
    size_t _size;
    StringType _data;
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(StringBuilderChunk)

} // namespace rendell_ui