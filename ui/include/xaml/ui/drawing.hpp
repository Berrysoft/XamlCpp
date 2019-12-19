#ifndef XAML_UI_DRAWING_HPP
#define XAML_UI_DRAWING_HPP

namespace xaml
{
    struct size
    {
        int width;
        int height;
    };

    struct point
    {
        int x;
        int y;
    };

    struct rectangle
    {
        int x, y;
        int width, height;
    };
} // namespace xaml

#endif // !XAML_UI_DRAWING_HPP
