#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/controls/button.hpp>

#include <CommCtrl.h>

using namespace std;

namespace xaml
{
    optional<LRESULT> button::__wnd_proc(window_message const& msg)
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
                    m_click(*this);
                    break;
                }
            }
        }
        }
        return nullopt;
    }

    void button::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            window_create_params params = {};
            params.class_name = WC_BUTTON;
            params.style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON;
            params.x = 0;
            params.y = 0;
            params.width = 100;
            params.height = 50;
            params.parent = get_parent().get();
            this->__create(params);
        }
        rectangle real = region - get_margin();
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, (int)real.x, (int)real.y, (int)real.width, (int)real.height, SWP_NOZORDER));
        __set_size_noevent({ real.width, real.height });
        draw_text();
        draw_default();
        SetParent(get_handle(), get_parent()->get_handle());
    }

    void button::draw_size()
    {
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, 0, 0, (int)get_width(), (int)get_height(), SWP_NOZORDER | SWP_NOMOVE));
    }

    void button::draw_text()
    {
        THROW_IF_WIN32_BOOL_FALSE(Button_SetText(get_handle(), m_text.c_str()));
    }

    void button::draw_default()
    {
        auto style = GetWindowLongPtr(get_handle(), GWL_STYLE);
        if (m_is_default)
            style |= BS_DEFPUSHBUTTON;
        else
            style &= ~BS_DEFPUSHBUTTON;
        Button_SetStyle(get_handle(), style, FALSE);
    }

    void button::__size_to_fit()
    {
        __set_size_noevent(__measure_text_size(m_text, { 10, 10 }));
        draw_size();
    }
} // namespace xaml
