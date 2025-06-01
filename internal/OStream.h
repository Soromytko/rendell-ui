#pragma once
#include <iostream>
#include <string>

namespace rendell_ui
{
	template<typename stream_type, typename string_type>
	class OStream final
	{
	public:
		OStream(stream_type& stream, string_type prefix) : _stream(stream), _prefix(std::move(prefix)) {}
		~OStream() = default;

		template <typename T>
		const OStream& operator<<(const T& value) const
		{
			_stream << _prefix << value;
			return *this;
		};

		const OStream& operator<<(std::ostream& (*manip)(std::ostream&)) const
		{
			_stream << manip;
			return *this;
		}

	private:
		stream_type& _stream;
		string_type _prefix;
	};

	inline const OStream debug(std::cout, "[RENDELL_UI::DEBUG] ");
	inline const OStream warning(std::cout, "[RENDELL_UI::WARNING] ");
	inline const OStream error(std::cerr, "[RENDELL_UI::ERROR] ");
	inline const OStream wdebug(std::wcout, L"[RENDELL_UI::DEBUG] ");
	inline const OStream wwarning(std::wcout, L"[RENDELL_UI::WARNING] ");
	inline const OStream werror(std::wcerr, L"[RENDELL_UI::ERROR] ");
}