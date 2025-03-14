#pragma once
#include <vector>
#include <string>
#include <rendell_text.h>
#include "Widget.h"
#include "private/TextRendererEditor.h"

namespace rendell_ui
{
	class TextEditWidget final : public Widget
	{
		FRIEND_WIDGET
	private:
		TextEditWidget();
	public:
		~TextEditWidget() = default;

		void draw() const override;

		void setText(const std::wstring& value);
		void setFontSize(glm::ivec2 value);

	private:
		void onSelfWeakPtrChanged() override;

		void setupTextEditor(const rendell_text::TextRendererSharedPtr& textRenderer);

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

		glm::ivec2 _fontSize{ glm::ivec2(24, 24) };
		std::vector<rendell_text::TextRendererSharedPtr> _lines{};
		TextRendererEditorSharedPtr _textEditor;
		size_t _currentRowIndex{ 0u };
		size_t _currentColumnIndex{ 0u };
	};

	DECLARE_WIDGET(TextEditWidget)

}

