#pragma once
#include <vector>
#include <string>
#include <rendell_text/rendell_text.h>
#include "Widget.h"
#include "IScrollableWidget.h"
#include "ScrollbarWidget.h"
#include "private/TextEditor.h"
#include "private/Cursor.h"
#include "private/TextDrawer.h"

namespace rendell_ui
{
	class TextEditWidget final : public Widget, public IScrollableWidget
	{
	public:
		TextEditWidget();
		~TextEditWidget();

		void draw() const override;

		// IScrollableWidget
		float getScrollProgress() const override;
		float getScrollRatio() const override;
		bool setScrollProgress(float value) override;

		const std::wstring& getText() const;
		size_t getLineCount() const;
		bool getScrollEnabled() const;

		void setText(const std::wstring& value);
		void setFontSize(glm::ivec2 value);
		void setScrollEnabled(bool value);

		Signal<void> textChanged;
		Signal<void, float> scrollProgressChanged;

	private:
		void onSelfWeakPtrChanged() override;
		void onDragged(glm::dvec2 startPoint, glm::dvec2 endPoint) override;
		void onSizeChanged() override;

		void onTextLayoutCleared();
		void onTextLayoutRemoved(size_t index);
		void onTextLayoutAdded(size_t index, const rendell_text::TextLayoutSharedPtr& textLayout);
		void onTextLayoutSwapped(size_t firstIndex, size_t secondIndex);
		void onCaretChanged(uint32_t x, uint32_t y, uint32_t height);

		void onFocused() override;
		void onUnfocused() override;
		void onMouseDown(glm::dvec2 cursorPosition) override;
		void onMouseScrolled(glm::dvec2 scroll) override;
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
		void processKeyX(InputModControl modControl);

		void updateCursorPosition();

		uint32_t _textLayoutClearedConnectionId;
		uint32_t _textLayoutRemovedConnectionId;
		uint32_t _textLayoutAddedConnectionId;
		uint32_t _textLayoutSwappedConnectionId;
		uint32_t _caretChangedConnectionId;
		uint32_t _textChangedConnectionId;

		bool _scrollEnabled{ true };

		glm::ivec2 _fontSize{ glm::ivec2(24, 24) };
		TextEditor _textEditor{};
		TextDrawer _textDrawer{};
		CursorSharedPtr _cursor{};
		ScrollbarWidgetSharedPtr _scrollbarWidget{};
	};

	RENDELL_UI_DECLARE_WIDGET(TextEditWidget)

}

