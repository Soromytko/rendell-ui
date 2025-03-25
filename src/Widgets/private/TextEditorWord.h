#pragma once
#include <memory>
#include <string>
#include "../../defines.h"

namespace rendell_ui
{
	class TextEditorWord
	{
	public:
		TextEditorWord(const std::wstring& characters = {});
		virtual ~TextEditorWord() = default;

		const std::wstring& getCharacters() const;

		virtual bool isWordCharacter(wchar_t character) const;

	private:
		std::wstring _characters;

	};
	RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(TextEditorWord)

	class DigitTextEditorWord final : public TextEditorWord
	{
	public:
		bool isWordCharacter(wchar_t character) const override;
	};
	RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(DigitTextEditorWord)

	class VerbalTextEditorWord final : public TextEditorWord
	{
	public:
		bool isWordCharacter(wchar_t character) const override;
	};
	RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(VerbalTextEditorWord)


}