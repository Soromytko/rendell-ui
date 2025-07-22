#pragma once
#include <string>
#include <vector>

namespace rendell_ui {
class StringExtension final {
private:
    StringExtension() = default;

public:
    static std::vector<std::wstring> split(const std::wstring &str, const std::wstring &delimiter);
};
} // namespace rendell_ui