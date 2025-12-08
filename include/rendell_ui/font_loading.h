#pragma once
#include <filesystem>
#include <memory>

namespace rendell_ui {
class IFont;

std::shared_ptr<IFont> loadFont(const std::filesystem::path &path);
} // namespace rendell_ui