#pragma once
#include <vector>
#include <string>
#include <rendell_text.h>
#include "Widget.h"
#include "private/TextRendererEditor.h"
#include "private/Cursor.h"
#include "../String/StringBuilder.h"

namespace rendell_ui
{
	class TextEditWidget final : public Widget
	{
		FRIEND_WIDGET
	private:
		TextEditWidget();
	public:
		~TextEditWidget();

		void draw() const override;

		const std::wstring& getText() const;

		void setText(const std::wstring& value);
		void setFontSize(glm::ivec2 value);

	private:
		void onSelfWeakPtrChanged() override;

		void onTextLayoutCleared();
		void onTextLayoutRemoved(size_t index);
		void onTextLayoutAdded(size_t index, const rendell_text::TextLayoutSharedPtr& textLayout);
		void onCaretChanged(uint32_t x, uint32_t y, uint32_t height);

		void onFocused() override;
		void onUnfocused() override;
		void onMouseDown(glm::dvec2 cursorPosition) override;
		void onKeyInputted(const KeyboardInput& keyboardInput) override;
		void onCharInputted(unsigned char character) override;

		void processKeyEnter(InputModControl modControl);
		void processKeyTab(InputModControl modControl);
		void processKeyBackspace(InputModControl modControl);
		void processKeyDelete(InputModControl modControl);
		void processKeyRight(InputModControl modControl);
		void processKeyLeft(InputModControl modControl);
		void processKeyDown(InputModControl modControl);
		void processKeyUp(InputModControl modControl);

		uint32_t _textLayoutClearedConnectionId;
		uint32_t _textLayoutRemovedConnectionId;
		uint32_t _textLayoutAddedConnectionId;
		uint32_t _caretChangedConnectionId;

		glm::ivec2 _fontSize{ glm::ivec2(24, 24) };
		TextEditor _textEditor{};
		CursorSharedPtr _cursor{};
		std::vector<rendell_text::TextRendererSharedPtr> _textRenderers{};
	};

	DECLARE_WIDGET(TextEditWidget)

}

