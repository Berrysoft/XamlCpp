#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/button.hpp>

using namespace std;

namespace xaml
{
    button::button() : common_control()
    {
        add_text_changed([this](button const&, string_view_t) {
            if (get_handle())
            {
                draw_text();
                __parent_redraw();
            }
        });
        add_size_changed([this](control const&, size) {
            if (get_handle())
            {
                draw_size();
                __parent_redraw();
            }
        });
        add_is_default_changed([this](button const&, bool) { if (get_handle()) draw_default(); });
    }

    button::~button() {}

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
            params.class_name = U("BUTTON");
            params.style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON;
            params.x = 0;
            params.y = 0;
            params.width = 100;
            params.height = 50;
            params.parent = get_parent().get();
            this->__create(params);
        }
        rectangle real = region - get_margin();
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, real.x, real.y, real.width, real.height, SWP_NOZORDER));
        __set_size_noevent({ real.width, real.height });
        draw_text();
        draw_default();
        SetParent(get_handle(), get_parent()->get_handle());
        ShowWindow(get_handle(), SW_SHOW);
    }

    void button::draw_size()
    {
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, 0, 0, get_width(), get_height(), SWP_NOZORDER | SWP_NOMOVE));
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
        size msize = __measure_text_size(m_text);
        __set_size_noevent({ msize.width + 15, msize.height + 15 });
        draw_size();
    }
} // namespace xaml
