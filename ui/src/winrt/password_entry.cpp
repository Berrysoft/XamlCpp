#include "winrt/Windows.UI.Xaml.Controls.h"
#include "winrt/Windows.UI.Xaml.h"
#include <winrt/drawing.hpp>
#include <xaml/ui/password_entry.hpp>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

namespace xaml
{
    void password_entry::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            PasswordBox t{};
            set_handle(t);
        }
        PasswordBox t = get_handle().as<PasswordBox>();
        t.Margin(get_Thickness(get_margin()));
        draw_size();
        draw_text();
    }

    void password_entry::draw_size()
    {
        PasswordBox t = get_handle().as<PasswordBox>();
        t.Width(get_width());
        t.Height(get_height());
    }

    void password_entry::draw_text()
    {
        PasswordBox t = get_handle().as<PasswordBox>();
        t.Password(m_text);
    }

    void password_entry::draw_password_char()
    {
        PasswordBox t = get_handle().as<PasswordBox>();
        if (m_password_char)
        {
            t.PasswordChar(hstring({ m_password_char }));
        }
        else
        {
            t.PasswordChar(hstring{});
        }
    }

    void password_entry::__size_to_fit()
    {
        PasswordBox t = get_handle().as<PasswordBox>();
        t.Measure({ INFINITY, INFINITY });
        set_size(xaml::get_size(t.DesiredSize()));
    }
} // namespace xaml
