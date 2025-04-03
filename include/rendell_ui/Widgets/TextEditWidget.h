#pragma once
#include <vector>
#include <string>
#include <rendell_text/rendell_text.h>
#include "Widget.h"
#include "IScrollableWidget.h"
#include "ScrollbarWidget.h"
#include "private/TextEditor.h"
#include "private/Cursor.h"

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
		void setScrollProgress(float value) override;

		const std::wstring& getText() const;
		size_t getLineCount() const;
		bool getScrollEnabled() const;

		void setText(const std::wstring& value);
		void setFontSize(glm::ivec2 value);
		void setScrollEnabled(bool value);

		Signal<void> textChanged;

	private:
		void onSelfWeakPtrChanged() override;
		void onDragged(glm::dvec2 startPoint, glm::dvec2 endPoint) override;

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

		bool updateScrollOffset(float value);
		void optimizeRendering();

		uint32_t _textLayoutClearedConnectionId;
		uint32_t _textLayoutRemovedConnectionId;
		uint32_t _textLayoutAddedConnectionId;
		uint32_t _textLayoutSwappedConnectionId;
		uint32_t _caretChangedConnectionId;
		uint32_t _textChangedConnectionId;

		float _scrollProgress{ 0.0f };
		float _scrollLength{ 0.0f };
		float _scrollOffset{ 0.0 };
		bool _scrollEnabled{ true };

		glm::ivec2 _fontSize{ glm::ivec2(24, 24) };
		TextEditor _textEditor{};
		CursorSharedPtr _cursor{};
		ScrollbarWidgetSharedPtr _scrollbarWidget{};
		std::vector<rendell_text::TextRendererSharedPtr> _textRenderers{};

		size_t _startRenderingIndex{ 0 };
		float _startRenderingOffset{ 0.0f };
		uint32_t _textHeight{0};
	};

	RENDELL_UI_DECLARE_WIDGET(TextEditWidget)

}

