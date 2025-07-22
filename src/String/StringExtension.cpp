#include "StringExtension.h"
#include <algorithm>

namespace rendell_ui {
std::vector<std::wstring> StringExtension::split(const std::wstring &str,
                                                 const std::wstring &delimiter) {
    std::vector<std::wstring> result;

    std::wstring::size_type pos = 0;
    std::wstring::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::wstring::npos) {
        result.push_back(str.substr(prev, pos - prev));
        prev = pos + delimiter.length();
    }

    result.push_back(str.substr(prev));

    return result;
}

} // namespace rendell_ui
