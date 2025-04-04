#pragma once
#include <memory>
#include <unordered_set>
#include <rendell/rendell.h>
#include <rendell_ui/defines.h>
#include <rendell_ui/Signal.h>
#include <rendell_ui/Widgets/IWidget.h>
#include <rendell_ui/Widgets/Anchor.h>
#include "../src/Widgets/Transform2D.h"

namespace rendell_ui
{
	class AnchorableWidget
	{
	protected:
		AnchorableWidget() = default;
		virtual ~AnchorableWidget() = default;

	public:
		const Transform2D& getTransform() const;
		glm::vec2 getOffset() const;
		glm::vec2 getSize() const;
		Margins getMargins() const;
		Anchor getAnchor() const;

		void setOffset(glm::vec2 value);
		void setSize(glm::vec2 value);
		void setMargins(Margins value);
		void setMargins(float left, float right, float bottom, float top);
		void setAnchor(Anchor value);

		bool intersect(glm::vec2 point) const;

		Signal<void, glm::vec2> offsetChanged;
		Signal<void, glm::vec2> sizeChanged;
		Signal<void, Margins> marginsChanged;
		Signal<void, Anchor> anchorChanged;

	protected:
		virtual void onOffsetChanged() {}
		virtual void onSizeChanged() {}
		virtual void onMarginsChanged() {}
		virtual void onAnchorChanged() {}
		
		bool setSizePrivate(glm::vec2 value);

		void updateMarkup();

		virtual void onMarkupUpdated() = 0;
		virtual glm::vec2 getRootPosition() const = 0;
		virtual glm::vec2 getRootSize() const = 0;

		Transform2D _transform{};
		glm::vec2 _offset{ 0.0f, 0.0f };
		glm::vec2 _size{ 100.0f, 100.0f };
		Margins _margins{ 0.0f, 0.0f, 0.0f, 0.0f };
		Anchor _anchor{ Anchor::center };
	};

}
