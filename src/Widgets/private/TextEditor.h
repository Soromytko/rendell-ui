#pragma once
#include <rendell_text.h>
#include <rendell_ui/Signal.h>
#include "TextEditorWord.h"
#include "../../String/StringBuilder.h"

namespace rendell_ui
{
	class TextEditor final
	{
		struct Caret
		{
			size_t x{};
			size_t y{};
			size_t xCorrector{};
			size_t offsetInString{};
		};

	public:
		TextEditor();
		~TextEditor() = default;

		const std::wstring& getText() const;
		size_t getCaretOffsetInString() const;
		uint32_t getCursorVerticalOffset() const;
		uint32_t getCursorHorizontalOffset() const;
		uint32_t getCursorHeight() const;
		const std::vector<TextEditorWordSharedPtr>& getWordTypes() const;

		void setText(const std::wstring& value);
		void setFontSize(glm::ivec2 value);
		void setWordTypes(const std::vector<TextEditorWordSharedPtr>& value);

		bool moveCursorToPrevChar(size_t count = 1);
		bool moveCursorToNextChar(size_t count = 1);
		bool moveCursorToPrevLine(size_t count = 1);
		bool moveCursorToNextLine(size_t count = 1);
		bool moveCursorToPrevWord();
		bool moveCursorToNextWord();
		bool moveCursorToStart();
		bool moveCursorToEnd();
		void setupCursorByOffset(double x, double y);

		bool eraseBeforeCursor(size_t count = 1);
		bool eraseAfterCursor(size_t count = 1);
		bool eraseWordBeforeCursor();
		bool eraseWordAfterCursor();
		bool eraseLineUnderCursor();
		bool insertAfterCursor(const std::wstring& text);
		bool moveLineUnderCursorDown();
		bool moveLineUnderCursorUp();

		Signal<void> textLayoutCleared;
		Signal<void, size_t> textLayoutRemoved;
		Signal<void, size_t, const rendell_text::TextLayoutSharedPtr&> textLayoutAdded;
		Signal<void, size_t, size_t> textLayoutSwapped;
		Signal<void, uint32_t, uint32_t, uint32_t> cursorChanged;

	private:
		std::wstring takeRemainingTextInLine(rendell_text::TextLayoutSharedPtr textLayout, size_t caretX, bool erase = false);
		std::wstring takeRemainingTextInLine(size_t caretX, size_t caretY, bool erase = false);

		void removeTextLayout(size_t index);
		void addTextLayout(size_t index, const rendell_text::TextLayoutSharedPtr& textLayout);
		void swipeLines(size_t firstIndex, size_t secondIndex);
		bool setCaret(size_t x, size_t y, bool setXCorrector = false);

		bool isSameWord(const TextEditorWordSharedPtr& word, wchar_t character) const;
		size_t getPrevWordLength() const;
		size_t getNextWordLength() const;
		size_t getCaretYByOffset(double offset) const;
		size_t getCaretXByOffset(size_t caretY, double offset) const;

		TextEditorWordSharedPtr findWord(wchar_t character) const;
		std::wstring convertLinesToString() const;

		Caret _caret{};
		StringBuilder _stringBuilder{};
		glm::ivec2 _fontSize{ glm::ivec2(24, 24) };
		std::vector<rendell_text::TextLayoutSharedPtr> _textLayouts;
		std::vector<TextEditorWordSharedPtr> _wordTypes;
		mutable std::wstring _cachedText{};
		mutable bool _shouldCachedTextBeUpdated{};
	};

}