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

		void processKey(int key, InputAction action, InputModControl modControl) override;
		void processChar(unsigned char character) override;

		glm::ivec2 _fontSize{ glm::ivec2(24, 24) };
		std::vector<rendell_text::TextRendererSharedPtr> _lines{};
		TextRendererEditor* _textEditor;
		size_t _currentRowIndex{ 0u };
		size_t _currentColumnIndex{ 0u };
	};

}

