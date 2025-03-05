#pragma once
#include <rendell_text.h>
#include "../Widget.h"
#include "Cursor.h"

namespace rendell_ui
{
	class TextRendererEditor final : public Widget
	{
		friend class TextEditWidget;

	protected:
		TextRendererEditor(Widget* parent, rendell_text::TextRendererSharedPtr textRenderer = nullptr);

	public:
		~TextRendererEditor() = default;

		size_t getCursorCharIndex() const;
		rendell_text::TextRendererSharedPtr getTextRenderer() const;
		void setTextRenderer(rendell_text::TextRendererSharedPtr value);
		void updateSize(bool shouldCursorOffsetBeRecalculated = false);

		bool moveCursorToPrevChar(uint32_t count = 1);
		bool moveCursorToNextChar(uint32_t count = 1);
		bool moveCursorToPrevWord();
		bool moveCursorToNextWord();
		bool moveCursorToPrevUntil(std::function<bool(wchar_t currentChar)> predicate);
		bool moveCursorToNextUntil(std::function<bool(wchar_t currentChar)> predicate);
		bool moveCursorToPrevUntil(const std::wstring& breakingCharacters);
		bool moveCursorToNextUntil(const std::wstring& breakingCharacters);
		bool moveCursorToStart();
		bool moveCursorToEnd();
		bool moveCursorToNearest(size_t charIndex);
		void setupCursorByOffset(double offset);

		bool eraseCursorChar();
		bool eraseWordBeforeCursor();
		bool eraseWordAfterCursor();
		bool eraseAllAfterCursor();
		bool insertCursorChar(unsigned char character);
		bool insertAfterCursor(const std::wstring& string);

	private:
		void recalculateCursorOffset();

		Cursor* _cursor;
		rendell_text::TextRendererSharedPtr _textRenderer;
		size_t _charIndex{ 0 };
	};
}