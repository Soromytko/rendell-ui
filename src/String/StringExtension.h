#pragma once
#include <vector>
#include <string>

namespace rendell_ui
{
	class StringExtension final
	{
	private:
		StringExtension() = default;

	public:
		static std::vector<std::wstring> split(const std::wstring& str, const std::wstring& delimiter);

	};
}