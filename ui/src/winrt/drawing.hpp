#ifndef XAML_INTERNAL_WINRT_DRAWING_HPP
#define XAML_INTERNAL_WINRT_DRAWING_HPP

#include "winrt/Windows.Foundation.Numerics.h"
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.UI.Xaml.h"
#include <xaml/ui/drawing.hpp>

namespace xaml
{
    inline winrt::Windows::UI::Xaml::Thickness get_Thickness(margin const& m) { return { m.left, m.top, m.right, m.bottom }; }

    inline winrt::Windows::Foundation::Size get_Size(size s) { return { (float)s.width, (float)s.height }; }
    inline size get_size(winrt::Windows::Foundation::Size const& s) { return { s.Width, s.Height }; }

    inline winrt::Windows::Foundation::Numerics::float2 get_float2(point p) { return { (float)p.x, (float)p.y }; }

    inline winrt::Windows::Foundation::Rect get_Rect(rectangle const& rect) { return { (float)rect.x, (float)rect.y, (float)rect.width, (float)rect.height }; }
} // namespace xaml

#endif // !XAML_INTERNAL_WINRT_DRAWING_HPP
