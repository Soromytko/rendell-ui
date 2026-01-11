#pragma once
#include <string>
#include <vector>

namespace rendell_ui {
class StringExtension final {
private:
    StringExtension() = default;

public:
    static std::vector<rendell_text::String> split(const rendell_text::String &str,
                                                   const rendell_text::String &delimiter);
};
} // namespace rendell_ui