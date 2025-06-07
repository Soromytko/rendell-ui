#include <logging.h>

namespace rendell_ui
{
	RUICLogger::RUICLogger(std::ostream& stream, std::string_view prefix, logx::Color color, std::string_view funcName) :
		logx::CLogger(stream, prefix, color), _funcName(funcName)
	{

	}

	RUICLogger::~RUICLogger()
	{
		this->operator<<(" (") << _funcName << ")";
	}

	RUIWLogger::RUIWLogger(std::wostream& stream, std::wstring_view prefix, logx::Color color, std::string& funcName) :
		logx::WLogger(stream, prefix, color), _funcName(funcName)
	{

	}

	RUIWLogger::~RUIWLogger()
	{
		this->operator<<(L" (") << _funcName.c_str() << L")";
	}

}