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
		double getScrollProgress() const override;
		double getScrollRatio() const override;
		bool setScrollProgress(double value) override;
		void onProcessMouseScrolled(glm::dvec2 scroll) override;

		uint32_t getTextHeight() const;
		glm::dvec2 getSize() const;
		double getScroll() const;
		const std::vector<rendell_text::TextRendererSharedPtr>& getLines() const;
		void draw(const glm::mat4 viewMatrix, const glm::mat4& modelMatrix) const;

		void setSize(glm::dvec2 value);

		void clear();
		void removeLine(size_t index);
		void addLine(size_t index, const rendell_text::TextRendererSharedPtr& textRenderer);
		void swapLines(size_t firstIndex, size_t secondIndex);

	private:
		void updateScroll();
		void optimizeRendering();

		size_t _startRenderingIndex{ 0 };
		double _startRenderingOffset{ 0.0 };
		uint32_t _textHeight{ 0 };

		glm::dvec2 _size{};
		double _scroll{};
		double _scrollProgress{};
		std::vector<rendell_text::TextRendererSharedPtr> _textRenderers;

	};
}