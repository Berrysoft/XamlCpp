#include "winrt/Windows.UI.Xaml.Controls.h"
#include "winrt/Windows.UI.Xaml.h"
#include <winrt/drawing.hpp>
#include <xaml/ui/label.hpp>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

namespace xaml
{
    void label::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            TextBlock t{};
            set_handle(t);
        }
        TextBlock t = get_handle().as<TextBlock>();
        t.Margin(get_Thickness(get_margin()));
        draw_size();
        draw_text();
        draw_alignment();
    }

    void label::draw_size()
    {
        if (get_size() != size{})
        {
            TextBlock t = get_handle().as<TextBlock>();
            t.Width(get_width());
            t.Height(get_height());
        }
    }

    void label::draw_text()
    {
        TextBlock t = get_handle().as<TextBlock>();
        t.Text(m_text);
    }

    void label::draw_alignment()
    {
        TextBlock t = get_handle().as<TextBlock>();
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

    void label::__size_to_fit()
    {
        TextBlock t = get_handle().as<TextBlock>();
        t.Measure({ INFINITY, INFINITY });
        set_size(xaml::get_size(t.DesiredSize()));
    }
} // namespace xaml
