#pragma once
#include <cmath>
#include <cstdint>
#include <tuple>

namespace rendell_ui {
enum class Anchor : uint8_t {
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

struct Margins {
    float left{0.0f};
    float right{0.0f};
    float bottom{0.0f};
    float top{0.0f};

    constexpr static Margins makeLeft(float value) { return {value, 0.0f, 0.0f, 0.0f}; }

    constexpr static Margins makeRight(float value) { return {0.0f, value, 0.0f, 0.0f}; }

    constexpr static Margins makeBottom(float value) { return {0.0f, 0.0f, value, 0.0f}; }

    constexpr static Margins makeTop(float value) { return {0.0f, 0.0f, 0.0f, value}; }

    constexpr static Margins makeHorizontal(float value) { return {value, value, 0.0f, 0.0f}; }

    constexpr static Margins makeVertical(float value) { return {0.0f, 0.0f, value, value}; }

    constexpr static Margins makeAll(float value) { return {value, value, value, value}; }

    constexpr Margins() = default;

    constexpr Margins(float left, float right, float bottom, float top)
        : left(left)
        , right(right)
        , bottom(bottom)
        , top(top) {}
};

inline bool operator==(const Margins &lhs, const Margins &rhs) {
    constexpr float epsilon = 1e-6f;

    return (std::abs(lhs.left - rhs.left) < epsilon) &&
           (std::abs(lhs.right - rhs.right) < epsilon) &&
           (std::abs(lhs.bottom - rhs.bottom) < epsilon) && (std::abs(lhs.top - rhs.top) < epsilon);
}

inline bool operator!=(const Margins &lhs, const Margins &rhs) {
    return !(lhs == rhs);
}
} // namespace rendell_ui
