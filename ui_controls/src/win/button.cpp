#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/controls/button.hpp>
#include <xaml/ui/native_control.hpp>

#include <CommCtrl.h>

using namespace std;

namespace xaml
{
    optional<std::intptr_t> button::__wnd_proc(window_message const& msg)
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
        if (auto sparent = get_parent().lock())
        {
            if (!get_handle())
            {
                window_create_params params = {};
                params.class_name = WC_BUTTON;
                params.style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON;
                params.x = 0;
                params.y = 0;
                params.width = 50;
                params.height = 14;
                params.parent = sparent.get();
                this->__create(params);
                draw_text();
                draw_default();
                SetParent(get_handle()->handle, sparent->get_handle()->handle);
            }
            __set_rect(region);
        }
    }

    void button::draw_text()
    {
        THROW_IF_WIN32_BOOL_FALSE(Button_SetText(get_handle()->handle, m_text.c_str()));
    }

    void button::draw_default()
    {
        auto style = GetWindowLongPtr(get_handle()->handle, GWL_STYLE);
        if (m_is_default)
            style |= BS_DEFPUSHBUTTON;
        else
            style &= ~BS_DEFPUSHBUTTON;
        Button_SetStyle(get_handle()->handle, style, FALSE);
    }

    void button::__size_to_fit()
    {
        __set_size_noevent(__measure_text_size(m_text, { 10, 10 }));
    }
} // namespace xaml
