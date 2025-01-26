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

		bool moveCursorToPrevChar();
		bool moveCursorToNextChar();
		bool moveCursorToStart();
		bool moveCursorToEnd();
		bool moveCursorToNearest(size_t charIndex);

	private:
		Cursor* _cursor;
		rendell_text::TextRendererSharedPtr _textRenderer;
		size_t _charIndex{ 0 };
	};
}