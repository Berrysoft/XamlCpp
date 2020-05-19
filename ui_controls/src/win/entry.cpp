#include <shared/entry.hpp>
#include <windowsx.h>
#include <xaml/result_win32.h>
#include <xaml/ui/controls/entry.h>

#include <CommCtrl.h>

using namespace std;

xaml_result xaml_entry_internal::draw(xaml_rectangle const& region) noexcept
{
    if (m_parent)
    {
        if (!m_handle)
        {
            xaml_win32_window_create_params params = {};
            params.class_name = WC_EDIT;
            params.style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL;
            params.ex_style = WS_EX_CLIENTEDGE;
            params.x = 0;
            params.y = 0;
            params.width = 100;
            params.height = 50;
            params.parent = m_parent;
            XAML_RETURN_IF_FAILED(create(params));
            XAML_RETURN_IF_FAILED(draw_visible());
            XAML_RETURN_IF_FAILED(draw_text());
            XAML_RETURN_IF_FAILED(draw_alignment());
        }
        XAML_RETURN_IF_FAILED(set_rect(region));
    }
    return XAML_S_OK;
}

xaml_result xaml_entry_internal::size_to_fit() noexcept
{
    xaml_size res;
    XAML_RETURN_IF_FAILED(measure_string(m_text, { 2, 2 }, &res));
    return set_size_noevent(res);
}

xaml_result xaml_entry_internal::draw_text() noexcept
{
    if (m_text)
    {
        xaml_char_t const* data;
        XAML_RETURN_IF_FAILED(m_text->get_data(&data));
        XAML_RETURN_IF_WIN32_BOOL_FALSE(Edit_SetText(m_handle, data));
    }
    else
    {
        XAML_RETURN_IF_WIN32_BOOL_FALSE(Edit_SetText(m_handle, nullptr));
    }
    return XAML_S_OK;
}

xaml_result xaml_entry_internal::draw_alignment() noexcept
{
    LONG_PTR style = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
    switch (m_text_halignment)
    {
    case xaml_halignment_center:
        style |= ES_CENTER;
        break;
    case xaml_halignment_right:
        style |= ES_RIGHT;
        break;
    default:
        style |= ES_LEFT;
        break;
    }
    SetWindowLongPtr(m_handle, GWL_STYLE, style);
    return XAML_S_OK;
}

xaml_result xaml_entry_internal::wnd_proc(xaml_win32_window_message const& msg, LRESULT*) noexcept
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
            case EN_UPDATE:
            {
                int len = Edit_GetTextLength(m_handle);
                xaml_std_string_t t(len, U('\0'));
                Edit_GetText(m_handle, t.data(), len + 1);
                DWORD start, end;
                SendMessage(m_handle, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
                xaml_ptr<xaml_string> text;
                XAML_RETURN_IF_FAILED(xaml_string_new(move(t), &text));
                XAML_RETURN_IF_FAILED(set_text(text));
                SetFocus(m_handle);
                Edit_SetSel(m_handle, start, end);
                break;
            }
            }
        }
    }
    }
    return XAML_E_NOTIMPL;
}
