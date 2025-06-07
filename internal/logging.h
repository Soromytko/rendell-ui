#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <logx/logx.h>

namespace rendell_ui
{
	class RUICLogger final : public logx::CLogger
	{
	public:
		RUICLogger(std::ostream& stream, std::string_view prefix, logx::Color color, std::string_view funcName);
		~RUICLogger();

	private:
		std::string_view _funcName;
	};

	class RUIWLogger final : public logx::WLogger
	{
	public:
		RUIWLogger(std::wostream& stream, std::wstring_view prefix, logx::Color color, std::string& funcName);
		~RUIWLogger();

	private:
		std::string& _funcName;
	};
}

#define RENDELL_UI_DEBUG_PREFIX "[RENDELL_UI::DEBUG] "
#define RENDELL_UI_ERROR_PREFIX "[RENDELL_UI::ERROR] "
#define RENDELL_UI_WARNING_PREFIX "[RENDELL_UI::WARNING] "

#define RENDELL_UI_DEBUG_WPREFIX L"[RENDELL_UI::DEBUG] "
#define RENDELL_UI_ERROR_WPREFIX L"[RENDELL_UI::ERROR] "
#define RENDELL_UI_WARNING_WPREFIX L"[RENDELL_UI::WARNING] "

#define RENDELL_UI_CLASS_FUNC std::string(LOGX_CLASS) + "::" + __func__

#define RUI_DEBUG rendell_ui::RUICLogger(std::cout, RENDELL_UI_DEBUG_PREFIX, LOGX_WHITE, RENDELL_UI_CLASS_FUNC)
#define RUI_ERROR rendell_ui::RUICLogger(std::cerr, RENDELL_UI_ERROR_PREFIX, LOGX_RED, RENDELL_UI_CLASS_FUNC)
#define RUI_WARNING rendell_ui::RUICLogger(std::cout, RENDELL_UI_WARNING_PREFIX, LOGX_YELLOW, RENDELL_UI_CLASS_FUNC)

#define RUI_W_DEBUG rendell_ui::RUIWLogger(std::wcout, RENDELL_UI_DEBUG_WPREFIX, LOGX_WHITE, RENDELL_UI_CLASS_FUNC)
#define RUI_W_ERROR rendell_ui::RUIWLogger(std::wcerr, RENDELL_UI_ERROR_WPREFIX, LOGX_RED, RENDELL_UI_CLASS_FUNC)
#define RUI_W_WARNING rendell_ui::RUIWLogger(std::wcout, RENDELL_UI_WARNING_WPREFIX, LOGX_YELLOW, RENDELL_UI_CLASS_FUNC)
