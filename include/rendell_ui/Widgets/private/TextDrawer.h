#pragma once
#include <rendell_text/rendell_text.h>
#include <rendell_ui/Widgets/IScrollableWidget.h>
#include <glm/glm.hpp>

namespace rendell_ui
{
	class TextDrawer final : public IScrollableWidget
	{
	public:
		TextDrawer();
		~TextDrawer() = default;

		// IScrollableWidget
		float getScrollProgress() const override;
		float getScrollRatio() const override;
		bool setScrollProgress(float value) override;
		void onProcessMouseScrolled(glm::dvec2 scroll) override;

		uint32_t getTextHeight() const;
		glm::vec2 getSize() const;
		float getScroll() const;
		const std::vector<rendell_text::TextRendererSharedPtr>& getLines() const;
		void draw(const glm::mat4 viewMatrix, const glm::mat4& modelMatrix) const;

		void setSize(glm::vec2 value);

		void clear();
		void removeLine(size_t index);
		void addLine(size_t index, const rendell_text::TextRendererSharedPtr& textRenderer);
		void swapLines(size_t firstIndex, size_t secondIndex);

	private:
		void updateScroll();
		void optimizeRendering();

		size_t _startRenderingIndex{ 0 };
		float _startRenderingOffset{ 0.0f };
		uint32_t _textHeight{ 0 };

		glm::vec2 _size{};
		float _scroll{};
		float _scrollProgress{};
		std::vector<rendell_text::TextRendererSharedPtr> _textRenderers;

	};
}