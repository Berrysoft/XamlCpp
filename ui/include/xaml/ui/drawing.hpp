#ifndef XAML_UI_DRAWING_HPP
#define XAML_UI_DRAWING_HPP

#include <cstdint>

namespace xaml
{
    struct size
    {
        double width;
        double height;
    };

    constexpr bool operator==(size lhs, size rhs) { return lhs.width == rhs.width && lhs.height == rhs.height; }
    constexpr bool operator!=(size lhs, size rhs) { return !(lhs == rhs); }

    struct point
    {
        double x;
        double y;
    };

    constexpr bool operator==(point lhs, point rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
    constexpr bool operator!=(point lhs, point rhs) { return !(lhs == rhs); }

    struct rectangle
    {
        double x, y;
        double width, height;
    };

    constexpr bool operator==(rectangle const& lhs, rectangle const& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.width == rhs.width && lhs.height == rhs.height; }
    constexpr bool operator!=(rectangle const& lhs, rectangle const& rhs) { return !(lhs == rhs); }

    struct margin
    {
        double left;
        double top;
        double right;
        double bottom;
    };

    constexpr bool operator==(margin const& lhs, margin const& rhs) { return lhs.left == rhs.left && lhs.top == rhs.top && lhs.right == rhs.right && lhs.bottom == rhs.bottom; }
    constexpr bool operator!=(margin const& lhs, margin const& rhs) { return !(lhs == rhs); }

    constexpr rectangle operator+(rectangle const& lhs, margin const& rhs) { return { lhs.x - rhs.left, lhs.y - rhs.top, lhs.width + rhs.left + rhs.right, lhs.height + rhs.top + rhs.bottom }; }
    constexpr rectangle operator-(rectangle const& lhs, margin const& rhs) { return { lhs.x + rhs.left, lhs.y + rhs.top, lhs.width - rhs.left - rhs.right, lhs.height - rhs.top - rhs.bottom }; }

    struct alignas(1) color
    {
        std::uint8_t a, r, g, b;
        constexpr operator std::int32_t() const noexcept { return ((std::int32_t)a << 24) + ((std::int32_t)r << 16) + ((std::int32_t)g << 8) + (std::int32_t)b; }
    };

    constexpr bool operator==(color lhs, color rhs) { return (std::int32_t)lhs == (std::int32_t)rhs; }
    constexpr bool operator!=(color lhs, color rhs) { return !(lhs == rhs); }
} // namespace xaml
#endif // !XAML_UI_DRAWING_HPP
