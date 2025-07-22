#include <cassert>
#include <logging.h>
#include <memory>
#include <unordered_map>

namespace rendell_ui {
std::unique_ptr<logx::Logger> s_logger{nullptr};

logx::Logger *get_logger() {
    if (!s_logger) {
        s_logger = std::make_unique<RUILogger>();
    }
    assert(s_logger);
    return s_logger.get();
}

RUILogger::RUILogger()
    : logx::Logger() {
}

const char *RUILogger::getLevelName_Unsafe(logx::Level level) const {
    static const std::unordered_map<logx::Level, const char *> levelNames{
        {logx::Level::critical, "RENDELL_UI::CRITICAL"}, {logx::Level::error, "RENDELL_UI::ERROR"},
        {logx::Level::warning, "RENDELL_UI::WARNING"},   {logx::Level::info, "RENDELL_UI::INFO"},
        {logx::Level::debug, "RENDELL_UI::DEBUG"},       {logx::Level::trace, "RENDELL_UI::TRACE"},
    };

    auto it = levelNames.find(level);
    if (it != levelNames.end()) {
        return it->second;
    }

    return nullptr;
}

} // namespace rendell_ui