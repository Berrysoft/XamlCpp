#include <windowsx.h>
#include <xaml/ui/controls/radio_box.hpp>
#include <xaml/ui/native_control.hpp>

#include <CommCtrl.h>
#include <Uxtheme.h>

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
            break;
        }
        }
        return nullopt;
    }

    void radio_box::__draw(rectangle const& region)
    {
        if (auto sparent = get_parent().lock())
        {
            if (!get_handle())
            {
                window_create_params params = {};
                params.class_name = WC_BUTTON;
                params.style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON;
                params.x = 0;
                params.y = 0;
                params.width = 50;
                params.height = 14;
                params.parent = sparent.get();
                this->__create(params);
                draw_visible();
                draw_text();
                draw_default();
                draw_checked();
                SetParent(get_handle()->handle, sparent->get_handle()->handle);
            }
            __set_rect(region);
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
