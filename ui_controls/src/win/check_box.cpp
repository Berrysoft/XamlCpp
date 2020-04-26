#include <shared/check_box.hpp>
#include <windowsx.h>
#include <xaml/ui/controls/check_box.h>

#include <CommCtrl.h>

using namespace std;

xaml_result xaml_check_box_impl::wnd_proc(xaml_win32_window_message const& msg, LRESULT* presult) noexcept
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
                XAML_RETURN_IF_FAILED(set_is_checked(Button_GetCheck(m_handle) == BST_CHECKED));
                break;
            }
        }
        break;
    }
    }
    return XAML_E_NOTIMPL;
}

xaml_result xaml_check_box_impl::draw(xaml_rectangle const& region) noexcept
{
    if (m_parent)
    {
        if (!m_handle)
        {
            xaml_win32_window_create_params params = {};
            params.class_name = WC_BUTTON;
            params.style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX;
            params.x = 0;
            params.y = 0;
            params.width = 50;
            params.height = 14;
            params.parent = m_parent;
            XAML_RETURN_IF_FAILED(create(params));
            XAML_RETURN_IF_FAILED(draw_visible());
            XAML_RETURN_IF_FAILED(draw_text());
            XAML_RETURN_IF_FAILED(draw_default());
            XAML_RETURN_IF_FAILED(draw_checked());
        }
        XAML_RETURN_IF_FAILED(set_rect(region));
    }
    return XAML_S_OK;
}

xaml_result xaml_check_box_impl::draw_checked() noexcept
{
    Button_SetCheck(m_handle, m_is_checked ? BST_CHECKED : BST_UNCHECKED);
}

xaml_result xaml_check_box_impl::int32_to_fit() noexcept
{
    xaml_size res;
    XAML_RETURN_IF_FAILED(measure_string(m_text, { 0, 5 }, &res));
    return set_size_noevent(res);
}
