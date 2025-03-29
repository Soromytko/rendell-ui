#pragma once
#include <cstdint>
#include <tuple>

namespace rendell_ui
{
	enum class Anchor : uint8_t
	{
		// Anchors
		left,
		right,
		bottom,
		top,
		center,
		leftBottom,
		leftTop,
		rightBottom,
		rightTop,

		// Stretch
		topStretch,
		horizontalStretch,
		bottomStretch,
		leftStretch,
		verticalStretch,
		rightStretch,
		centerStretch,
	};

	struct Margins
	{
		float left;
		float right;
		float bottom;
		float top;
	};

	inline bool operator==(const Margins& lhs, const Margins& rhs)
	{
		return std::tie(lhs.left, lhs.right, lhs.bottom, lhs.top) == std::tie(rhs.left, rhs.right, rhs.bottom, rhs.top);
	}
}
