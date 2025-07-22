#pragma once
#include <iostream>
#include <logx/logx.h>

namespace rendell_ui {
class RUILogger final : public logx::Logger {
public:
    RUILogger();

private:
    const char *getLevelName_Unsafe(logx::Level level) const override;
};

logx::Logger *get_logger();

} // namespace rendell_ui

#define RUI_CRITICAL(formatStr, ...)                                                               \
    rendell_ui::get_logger()->critical(std::format(formatStr, ##__VA_ARGS__))
#define RUI_ERROR(formatStr, ...)                                                                  \
    rendell_ui::get_logger()->error(std::format(formatStr, ##__VA_ARGS__))
#define RUI_WARNING(formatStr, ...)                                                                \
    rendell_ui::get_logger()->warning(std::format(formatStr, ##__VA_ARGS__))
#define RUI_INFO(formatStr, ...)                                                                   \
    rendell_ui::get_logger()->info(std::format(formatStr, ##__VA_ARGS__))
#define RUI_DEBUG(formatStr, ...)                                                                  \
    rendell_ui::get_logger()->debug(std::format(formatStr, ##__VA_ARGS__))
#define RUI_TRACE(formatStr, ...)                                                                  \
    rendell_ui::get_logger()->trace(std::format(formatStr, ##__VA_ARGS__))
