#ifndef XAML_UI_DRAWING_HPP
#define XAML_UI_DRAWING_HPP

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

    struct rectangle
    {
        double x, y;
        double width, height;
    };

    struct margin
    {
        double left;
        double top;
        double right;
        double bottom;
    };

    constexpr bool operator==(margin const& lhs, margin const& rhs) { return lhs.left == rhs.left && lhs.top == rhs.top && lhs.right == rhs.right && lhs.bottom == rhs.bottom; }
    constexpr bool operator!=(margin const& lhs, margin const& rhs) { return !(lhs == rhs); }
} // namespace xaml
#endif // !XAML_UI_DRAWING_HPP
