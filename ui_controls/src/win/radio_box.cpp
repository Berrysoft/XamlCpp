#include <windowsx.h>
#include <xaml/ui/controls/radio_box.hpp>

using namespace std;

namespace xaml
{
    optional<LRESULT> radio_box::__wnd_proc(window_message const& msg)
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
        button::__draw(region);
        LONG_PTR style = GetWindowLongPtr(get_handle(), GWL_STYLE);
        style |= BS_RADIOBUTTON;
        SetWindowLongPtr(get_handle(), GWL_STYLE, style);
        draw_checked();
    }

    void radio_box::draw_checked()
    {
        Button_SetCheck(get_handle(), m_is_checked ? BST_CHECKED : BST_UNCHECKED);
    }

    void radio_box::__size_to_fit()
    {
        __set_size_noevent(__measure_text_size(get_text(), { 10, 5 }));
        draw_size();
    }
} // namespace xaml
