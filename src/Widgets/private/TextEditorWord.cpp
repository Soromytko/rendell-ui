#include "TextEditorWord.h"

namespace rendell_ui
{
	TextEditorWord::TextEditorWord(const std::wstring& characters) : _characters(characters)
	{

	}

	const std::wstring& TextEditorWord::getCharacters() const
	{
		return _characters;
	}
	
	bool TextEditorWord::isWordCharacter(wchar_t character) const
	{
		return _characters.find(character) != std::wstring::npos;
	}

	bool DigitTextEditorWord::isWordCharacter(wchar_t character) const
	{
		return std::isdigit(static_cast<int>(character));
	}

	static bool is_english_latter(wchar_t character)
	{
		return (character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z');
	}

	bool VerbalTextEditorWord::isWordCharacter(wchar_t character) const
	{
		return is_english_latter(character);
	}


	
}

