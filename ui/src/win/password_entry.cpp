#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/password_entry.hpp>

namespace xaml
{
    XAML_API void password_entry::__draw(rectangle const& region)
    {
        entry::__draw(region);
        auto style = GetWindowLongPtr(get_handle(), GWL_STYLE);
        style |= ES_PASSWORD;
        SetWindowLongPtr(get_handle(), GWL_STYLE, style);
        draw_password_char();
    }

    XAML_API void password_entry::draw_password_char()
    {
        Edit_SetPasswordChar(get_handle(), m_password_char ? m_password_char : U('*'));
    }
} // namespace xaml
