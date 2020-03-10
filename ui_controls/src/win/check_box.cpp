#include <wil/resource.h>
#include <windowsx.h>
#include <xaml/ui/controls/check_box.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    optional<std::intptr_t> check_box::__wnd_proc(window_message const& msg)
    {
        switch (msg.Msg)
        {
        case WM_COMMAND:
        {
            HWND h = (HWND)msg.lParam;
            if (get_handle() && get_handle()->handle == h)
            {
                switch (HIWORD(msg.wParam))
                {
                case BN_CLICKED:
                    set_is_checked(Button_GetCheck(get_handle()->handle) == BST_CHECKED);
                    break;
                }
            }
            break;
        }
        }
        return nullopt;
    }

    void check_box::__draw(rectangle const& region)
    {
        bool new_draw = !get_handle();
        button::__draw(region);
        if (new_draw)
        {
            LONG_PTR style = GetWindowLongPtr(get_handle()->handle, GWL_STYLE);
            style |= BS_AUTOCHECKBOX;
            SetWindowLongPtr(get_handle()->handle, GWL_STYLE, style);
            draw_checked();
        }
    }

    void check_box::draw_checked()
    {
        Button_SetCheck(get_handle()->handle, m_is_checked ? BST_CHECKED : BST_UNCHECKED);
    }

    void check_box::__size_to_fit()
    {
        __set_size_noevent(__measure_text_size(get_text(), { 0, 5 }));
    }
} // namespace xaml
