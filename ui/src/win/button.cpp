#include <wil/result_macros.h>
#include <xaml/ui/button.hpp>

namespace xaml
{
    button::button() : common_control()
    {
        add_text_changed([this](button const&, string_view_t t) { if(get_handle()) THROW_IF_WIN32_BOOL_FALSE(SetWindowText(get_handle(), t.data())); });
    }

    LRESULT button::wnd_proc(window_message const& msg)
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
                case BN_DOUBLECLICKED:
                    m_dbclick(*this);
                    break;
                }
            }
        }
        }
        return common_control::wnd_proc(msg);
    }

    void button::draw(rectangle const& region)
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
            this->create(params);
        }
        int x = region.x + get_margin().left;
        int y = region.y + get_margin().top;
        int width = region.width - get_margin().left - get_margin().right;
        int height = region.height - get_margin().top - get_margin().bottom;
        THROW_IF_WIN32_BOOL_FALSE(MoveWindow(get_handle(), x, y, width, height, TRUE));
        THROW_IF_WIN32_BOOL_FALSE(SetWindowText(get_handle(), m_text.c_str()));
        auto style = GetWindowLongPtr(get_handle(), GWL_STYLE);
        if (m_is_default)
            style |= BS_DEFPUSHBUTTON;
        else
            style &= ~BS_DEFPUSHBUTTON;
        SetWindowLongPtr(get_handle(), GWL_STYLE, style);
    }

    button::~button() {}
} // namespace xaml
