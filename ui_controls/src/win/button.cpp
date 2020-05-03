#include <shared/button.hpp>
#include <windowsx.h>
#include <xaml/result_win32.h>
#include <xaml/ui/controls/button.h>
#include <xaml/ui/win/control.h>

#include <CommCtrl.h>

using namespace std;

xaml_result xaml_button_internal::draw(xaml_rectangle const& region) noexcept
{
    if (m_parent)
    {
        if (!m_handle)
        {
            xaml_win32_window_create_params params = {};
            params.class_name = WC_BUTTON;
            params.style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON;
            params.x = 0;
            params.y = 0;
            params.width = 50;
            params.height = 14;
            params.parent = m_parent;
            XAML_RETURN_IF_FAILED(create(params));
            XAML_RETURN_IF_FAILED(draw_visible());
            XAML_RETURN_IF_FAILED(draw_text());
            XAML_RETURN_IF_FAILED(draw_default());
        }
        XAML_RETURN_IF_FAILED(set_rect(region));
    }
    return XAML_S_OK;
}

xaml_result xaml_button_internal::draw_text() noexcept
{
    if (m_text)
    {
        xaml_char_t const* data;
        XAML_RETURN_IF_FAILED(m_text->get_data(&data));
        XAML_RETURN_IF_WIN32_BOOL_FALSE(Button_SetText(m_handle, data));
    }
    else
    {
        XAML_RETURN_IF_WIN32_BOOL_FALSE(Button_SetText(m_handle, nullptr));
    }
    return XAML_S_OK;
}

xaml_result xaml_button_internal::draw_default() noexcept
{
    auto style = GetWindowLongPtr(m_handle, GWL_STYLE);
    if (m_is_default)
        style |= BS_DEFPUSHBUTTON;
    else
        style &= ~BS_DEFPUSHBUTTON;
    Button_SetStyle(m_handle, style, FALSE);
    return XAML_S_OK;
}

xaml_result xaml_button_internal::wnd_proc(xaml_win32_window_message const& msg, LRESULT*) noexcept
{
    switch (msg.Msg)
    {
    case WM_COMMAND:
    {
        HWND h = (HWND)msg.lParam;
        if (m_handle == h)
        {
            switch (HIWORD(msg.wParam))
            {
            case BN_CLICKED:
                on_click(static_cast<xaml_button*>(m_outer_this));
                break;
            }
        }
    }
    }
    return XAML_E_NOTIMPL;
}

xaml_result xaml_button_internal::size_to_fit() noexcept
{
    xaml_size res;
    XAML_RETURN_IF_FAILED(measure_string(m_text, { 5, 5 }, &res));
    return set_size_noevent(res);
}
