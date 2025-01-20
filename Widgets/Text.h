#pragma once
#include <rendell_text.h>
#include "Widget.h"

class Text : public Widget
{
public:
	Text(Widget* parent = nullptr);
	~Text() = default;

	void setColor(glm::vec4 value) override;
	void draw() const override;

	void setFontPath(const std::string& value);
	void setFontSize(glm::ivec2 value);
	void setBackgroundColor(glm::vec4 value);
	void setText(const std::wstring& value);

	const std::string& getFontPath() const;
	glm::ivec2 getFontSize() const;
	//const glm::vec4& getBackgroundColor() const;
	const std::wstring& getText() const;

private:
	rendell_text::TextRendererSharedPtr _textRenderer{ nullptr };
};

typedef std::shared_ptr<Text> TextSharedPtr;
