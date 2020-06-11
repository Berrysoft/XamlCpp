#include <shared/atomic_guard.hpp>
#include <shared/text_box.hpp>
#include <windowsx.h>
#include <xaml/internal/string.hpp>
#include <xaml/result_win32.h>

#include <CommCtrl.h>

using namespace std;

xaml_result xaml_text_box_internal::draw(xaml_rectangle const& region) noexcept
{
    if (m_parent)
    {
        if (!m_handle)
        {
            xaml_win32_window_create_params params = {};
            params.class_name = WC_EDIT;
            params.style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL;
            params.ex_style = WS_EX_CLIENTEDGE;
            params.x = 0;
            params.y = 0;
            params.width = 100;
            params.height = 50;
            params.parent = m_parent;
            XAML_RETURN_IF_FAILED(create(params));
            XAML_RETURN_IF_WIN32_BOOL_FALSE(ShowScrollBar(m_handle, SB_VERT, TRUE));
            XAML_RETURN_IF_FAILED(draw_visible());
            XAML_RETURN_IF_FAILED(draw_text());
        }
        XAML_RETURN_IF_FAILED(set_rect(region));
    }
    return XAML_S_OK;
}

xaml_result xaml_text_box_internal::size_to_fit() noexcept
{
    xaml_size res;
    XAML_RETURN_IF_FAILED(measure_string(m_text, { 2, 2 }, &res));
    return set_size_noevent(res);
}

xaml_result xaml_text_box_internal::draw_text() noexcept
{
    wstring data;
    XAML_RETURN_IF_FAILED(to_wstring(m_text, &data));
    for (size_t i = 0; i < data.size(); i++)
    {
        if (data[i] == L'\n' && i > 0 && data[i - 1] != L'\r')
        {
            data.insert(i, 1, L'\r');
            i++;
        }
    }
    XAML_RETURN_IF_WIN32_BOOL_FALSE(Edit_SetText(m_handle, data.c_str()));
    return XAML_S_OK;
}

xaml_result xaml_text_box_internal::wnd_proc(xaml_win32_window_message const& msg, LRESULT*) noexcept
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
                break;
            }
            }
        }
    }
    }
    return XAML_E_NOTIMPL;
}
