#pragma once
#include <cstdint>
#include <tuple>
#include <cmath>

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
		constexpr float epsilon = 1e-6f;

		return (std::abs(lhs.left - rhs.left) < epsilon) &&
			(std::abs(lhs.right - rhs.right) < epsilon) &&
			(std::abs(lhs.bottom - rhs.bottom) < epsilon) &&
			(std::abs(lhs.top - rhs.top) < epsilon);
	}

	inline bool operator!=(const Margins& lhs, const Margins& rhs)
	{
		return !(lhs == rhs);
	}
}
