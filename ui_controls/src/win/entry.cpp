#include <shared/atomic_guard.hpp>
#include <shared/entry.hpp>
#include <windowsx.h>
#include <xaml/internal/string.hpp>
#include <xaml/result_win32.h>

#include <CommCtrl.h>

using namespace std;

xaml_result xaml_entry_internal::draw(xaml_rectangle const& region) noexcept
{
    xaml_ptr<xaml_element_base> parent;
    XAML_RETURN_IF_FAILED(get_parent(&parent));
    if (parent)
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
            params.parent = parent;
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
    nowide::wstackstring data;
    XAML_RETURN_IF_FAILED(to_wstring(m_text, &data));
    XAML_RETURN_IF_WIN32_BOOL_FALSE(Edit_SetText(m_handle, data.c_str()));
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
                xaml_atomic_guard guard{ m_text_changing };
                guard.test_and_set();
                int len = Edit_GetTextLength(m_handle);
                wstring t(len, L'\0');
                Edit_GetText(m_handle, t.data(), len + 1);
                xaml_ptr<xaml_string> text;
                XAML_RETURN_IF_FAILED(xaml_string_new(t, &text));
                XAML_RETURN_IF_FAILED(set_text(text));
                XAML_RETURN_IF_FAILED(parent_redraw());
                break;
            }
            }
        }
    }
    }
    return XAML_E_NOTIMPL;
}
