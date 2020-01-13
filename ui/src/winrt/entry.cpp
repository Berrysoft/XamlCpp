#include "winrt/Windows.UI.Xaml.Controls.h"
#include "winrt/Windows.UI.Xaml.h"
#include <winrt/drawing.hpp>
#include <xaml/ui/entry.hpp>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

namespace xaml
{
    XAML_API void entry::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            TextBox t{};
            t.AcceptsReturn(false);
            set_handle(t);
        }
        TextBox t = get_handle().as<TextBox>();
        t.Margin(get_Thickness(get_margin()));
        draw_size();
        draw_text();
        draw_alignment();
    }

    XAML_API void entry::draw_size()
    {
        TextBox t = get_handle().as<TextBox>();
        t.Width(get_width());
        t.Height(get_height());
    }

    XAML_API void entry::draw_text()
    {
        TextBox t = get_handle().as<TextBox>();
        t.Text(m_text);
    }

    XAML_API void entry::draw_alignment()
    {
        TextBox t = get_handle().as<TextBox>();
        switch (m_text_halignment)
        {
        case halignment_t::left:
            t.TextAlignment(TextAlignment::Left);
            break;
        case halignment_t::center:
            t.TextAlignment(TextAlignment::Center);
            break;
        case halignment_t::right:
            t.TextAlignment(TextAlignment::Right);
            break;
        default:
            t.TextAlignment(TextAlignment::Justify);
            break;
        }
    }

    XAML_API void entry::__size_to_fit()
    {
        TextBox t = get_handle().as<TextBox>();
        t.Measure({ INFINITY, INFINITY });
        set_size(xaml::get_size(t.DesiredSize()));
    }
} // namespace xaml
