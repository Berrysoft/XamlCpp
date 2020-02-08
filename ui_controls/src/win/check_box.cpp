#include <windowsx.h>
#include <xaml/ui/controls/check_box.hpp>

using namespace std;

namespace xaml
{
    check_box::check_box() : button()
    {
        add_is_checked_changed([this](check_box const&, bool) { if(get_handle()) draw_checked(); });
    }

    check_box::~check_box() {}

    optional<LRESULT> check_box::__wnd_proc(window_message const& msg)
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
                    set_is_checked(Button_GetCheck(get_handle()) == BST_CHECKED);
                    break;
                }
            }
        }
        }
        return nullopt;
    }

    void check_box::__draw(rectangle const& region)
    {
        button::__draw(region);
        LONG_PTR style = GetWindowLongPtr(get_handle(), GWL_STYLE);
        style |= BS_AUTOCHECKBOX;
        SetWindowLongPtr(get_handle(), GWL_STYLE, style);
        draw_checked();
    }

    void check_box::draw_checked()
    {
        Button_SetCheck(get_handle(), m_is_checked ? BST_CHECKED : BST_UNCHECKED);
    }

    void check_box::__size_to_fit()
    {
        size msize = __measure_text_size(get_text());
        __set_size_noevent({ msize.width + 20, msize.height + 10 });
        draw_size();
    }
} // namespace xaml
