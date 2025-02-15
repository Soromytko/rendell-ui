#pragma once
#include <vector>
#include <string>
#include <rendell_text.h>
#include "Widget.h"
#include "private/TextRendererEditor.h"

namespace rendell_ui
{
	class TextEdit final : public Widget
	{
	public:
		TextEdit(Widget* parent = nullptr);
		~TextEdit() = default;

		void draw() const override;

		void setText(const std::wstring& value);
		void setFontSize(glm::ivec2 value);

	private:
		void setupTextEditor();

		void processKey(InputKey key, InputAction action, InputModControl modControl) override;
		void processMouseButton(const MouseInput& mouseInput) override;

		void processKeyEnter(InputModControl modControl);
		void processKeyTab(InputModControl modControl);
		void processKeyBackspace(InputModControl modControl);
		void processKeyDelete(InputModControl modControl);
		void processKeyRight(InputModControl modControl);
		void processKeyLeft(InputModControl modControl);
		void processKeyDown(InputModControl modControl);
		void processKeyUp(InputModControl modControl);

		void processChar(unsigned char character) override;

		glm::ivec2 _fontSize{ glm::ivec2(24, 24) };
		std::vector<rendell_text::TextRendererSharedPtr> _lines{};
		TextRendererEditor* _textEditor;
		size_t _currentRowIndex{ 0u };
		size_t _currentColumnIndex{ 0u };
	};

}

