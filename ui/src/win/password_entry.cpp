#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/password_entry.hpp>

namespace xaml
{
    static char_t default_char{ U('*') };

    XAML_API void password_entry::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            window_create_params params = {};
            params.class_name = U("EDIT");
            params.style = WS_CHILD | WS_VISIBLE | ES_LEFT | ES_PASSWORD;
            params.ex_style = WS_EX_CLIENTEDGE;
            params.x = 0;
            params.y = 0;
            params.width = 100;
            params.height = 50;
            params.parent = get_parent().get();
            this->__create(params);
            default_char = Edit_GetPasswordChar(get_handle());
        }
        rectangle real = region - get_margin();
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, (int)real.x, (int)real.y, (int)real.width, (int)real.height, SWP_NOZORDER));
        __set_size_noevent({ real.width, real.height });
        draw_text();
        draw_password_char();
        SetParent(get_handle(), get_parent()->get_handle());
        ShowWindow(get_handle(), SW_SHOW);
    }

    XAML_API void password_entry::draw_size()
    {
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, 0, 0, (int)get_width(), (int)get_height(), SWP_NOZORDER | SWP_NOMOVE));
    }

    XAML_API void password_entry::draw_text()
    {
        THROW_IF_WIN32_BOOL_FALSE(Edit_SetText(get_handle(), m_text.c_str()));
    }

    XAML_API void password_entry::draw_password_char()
    {
        Edit_SetPasswordChar(get_handle(), m_password_char ? m_password_char : default_char);
    }

    XAML_API void password_entry::__size_to_fit()
    {
        size msize = __measure_text_size(m_text);
        __set_size_noevent({ msize.width + 15, msize.height + 15 });
        draw_size();
    }
} // namespace xaml
