#include "StringExtension.h"
#include <algorithm>

namespace rendell_ui {
std::vector<rendell_text::String> StringExtension::split(const rendell_text::String &str,
                                                         const rendell_text::String &delimiter) {
    std::vector<rendell_text::String> result;

    rendell_text::String::size_type pos = 0;
    rendell_text::String::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != rendell_text::String::npos) {
        result.push_back(str.substr(prev, pos - prev));
        prev = pos + delimiter.length();
    }

    result.push_back(str.substr(prev));

    return result;
}

} // namespace rendell_ui
