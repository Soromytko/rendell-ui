#pragma once
#include <rendell_text.h>
#include "../Widget.h"
#include "Cursor.h"

namespace rendell_ui
{
	class TextRendererEditor final : public Widget
	{
		friend class TextEdit;

	protected:
		TextRendererEditor(Widget* parent, rendell_text::TextRendererSharedPtr textRenderer = nullptr);

	public:
		~TextRendererEditor() = default;

		size_t getCursorCharIndex() const;
		
		void setTextRenderer(rendell_text::TextRendererSharedPtr value);
		void updateSize();

		bool moveCursorToPrevChar(uint32_t count = 1);
		bool moveCursorToNextChar(uint32_t count = 1);
		bool moveCursorToStart();
		bool moveCursorToEnd();
		bool moveCursorToNearest(size_t charIndex);

		bool eraseCursorChar();
		bool eraseAllAfterCursor();
		bool insertCursorChar(unsigned char character);
		bool insertAfterCursor(const std::wstring& string);

	private:
		Cursor* _cursor;
		rendell_text::TextRendererSharedPtr _textRenderer;
		size_t _charIndex{ 0 };
	};
}