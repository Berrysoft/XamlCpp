#include <windowsx.h>
#include <xaml/ui/controls/radio_box.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    optional<std::intptr_t> radio_box::__wnd_proc(window_message const& msg)
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
                    set_is_checked(true);
                    break;
                }
            }
        }
        }
        return nullopt;
    }

    void radio_box::__draw(rectangle const& region)
    {
        bool new_draw = !get_handle();
        button::__draw(region);
        if (new_draw)
        {
            LONG_PTR style = GetWindowLongPtr(get_handle()->handle, GWL_STYLE);
            style |= BS_RADIOBUTTON;
            SetWindowLongPtr(get_handle()->handle, GWL_STYLE, style);
            draw_checked();
        }
    }

    void radio_box::draw_checked()
    {
        Button_SetCheck(get_handle()->handle, m_is_checked ? BST_CHECKED : BST_UNCHECKED);
    }

    void radio_box::__size_to_fit()
    {
        __set_size_noevent(__measure_text_size(get_text(), { 0, 5 }));
    }
} // namespace xaml
