#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "winrt/Windows.UI.Xaml.Controls.h"
#include "winrt/Windows.UI.Xaml.h"
#include <winrt/drawing.hpp>
#include <xaml/ui/button.hpp>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

namespace xaml
{
    XAML_API button_base::button_base() : common_control()
    {
        add_text_changed([this](button_base const&, string_view_t) {
            if (get_handle())
            {
                draw_text();
            }
        });
        add_size_changed([this](control const&, size) {
            if (get_handle())
            {
                draw_size();
            }
        });
    }

    XAML_API button_base::~button_base()
    {
    }

    XAML_API void button_base::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            Button b{};
            b.Click([this](IInspectable const&, RoutedEventArgs const&) -> void {
                m_click(*this);
            });
            set_handle(b);
        }
        Button b = get_handle().as<Button>();
        b.Margin(get_Thickness(get_margin()));
        draw_size();
        draw_text();
    }

    XAML_API void button_base::draw_text()
    {
        Button b = get_handle().as<Button>();
        b.Content(box_value(m_text));
    }

    XAML_API void button_base::draw_size()
    {
        Button b = get_handle().as<Button>();
        b.Width(get_width());
        b.Height(get_height());
    }

    XAML_API void button_base::draw_default() {}

    XAML_API void button_base::__size_to_fit()
    {
        Button b = get_handle().as<Button>();
        b.Measure({ INFINITY, INFINITY });
        set_size(xaml::get_size(b.DesiredSize()));
    }
} // namespace xaml
