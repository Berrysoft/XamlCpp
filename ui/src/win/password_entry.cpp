#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/password_entry.hpp>

using namespace std;

namespace xaml
{
    XAML_API optional<LRESULT> password_entry::__wnd_proc(window_message const& msg)
    {
        switch (msg.Msg)
        {
        case WM_COMMAND:
        {
            HWND h = (HWND)msg.lParam;
            if (get_handle() == h)
            {
                switch (HIWORD(msg.wParam))
                {
                case EN_UPDATE:
                {
                    int len = Edit_GetTextLength(get_handle());
                    string_t t(len, U('\0'));
                    Edit_GetText(get_handle(), t.data(), len + 1);
                    DWORD start, end;
                    SendMessage(get_handle(), EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
                    set_text(t);
                    SetFocus(get_handle());
                    Edit_SetSel(get_handle(), start, end);
                    break;
                }
                }
            }
        }
        }
        return nullopt;
    }

    static char_t default_char{ U('*') };

    XAML_API void password_entry::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            window_create_params params = {};
            params.class_name = U("EDIT");
            params.style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL | ES_PASSWORD;
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
        __set_size_noevent({ msize.width + 5, msize.height + 10 });
        draw_size();
    }
} // namespace xaml
