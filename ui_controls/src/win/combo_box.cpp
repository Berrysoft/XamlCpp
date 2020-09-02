#include <algorithm>
#include <shared/combo_box.hpp>
#include <windowsx.h>
#include <xaml/internal/string.hpp>
#include <xaml/result_win32.h>
#include <xaml/ui/controls/combo_box.h>
#include <xaml/ui/win/dpi.h>

#include <CommCtrl.h>

using namespace std;

xaml_result xaml_combo_box_internal::wnd_proc(xaml_win32_window_message const& msg, LRESULT*) noexcept
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
            case CBN_SELCHANGE:
            {
                XAML_RETURN_IF_FAILED(set_sel_id(ComboBox_GetCurSel(m_handle)));
                int count = ComboBox_GetTextLength(m_handle);
                wstring text(count, L'\0');
                ComboBox_GetText(m_handle, text.data(), count + 1);
                xaml_ptr<xaml_string> s;
                XAML_RETURN_IF_FAILED(xaml_string_new(text, &s));
                XAML_RETURN_IF_FAILED(set_text(s));
                break;
            }
            }
        }
    }
    }
    return XAML_E_NOTIMPL;
}

xaml_result xaml_combo_box_internal::draw(xaml_rectangle const& region) noexcept
{
    xaml_ptr<xaml_element_base> parent;
    XAML_RETURN_IF_FAILED(get_parent(&parent));
    if (parent)
    {
        if (!m_handle)
        {
            XAML_RETURN_IF_FAILED(draw_editable());
        }
        XAML_RETURN_IF_FAILED(set_rect(region));
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_text() noexcept
{
    if (m_text)
    {
        nowide::wstackstring data;
        XAML_RETURN_IF_FAILED(to_wstring(m_text, &data));
        XAML_RETURN_IF_WIN32_BOOL_FALSE(ComboBox_SetText(m_handle, data.c_str()));
    }
    else
    {
        XAML_RETURN_IF_WIN32_BOOL_FALSE(ComboBox_SetText(m_handle, nullptr));
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_items() noexcept
{
    if (m_items)
    {
        xaml_codecvt_pool pool;
        XAML_FOREACH_START(item, m_items);
        {
            XAML_RETURN_IF_FAILED(create_item(item));
            xaml_ptr<xaml_string> s = item.query<xaml_string>();
            if (s)
            {
                wstring_view data;
                XAML_RETURN_IF_FAILED(pool(s, &data));
                ComboBox_AddString(m_handle, data.data());
            }
        }
        XAML_FOREACH_END();
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_sel() noexcept
{
    ComboBox_SetCurSel(m_handle, m_sel_id);
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::draw_editable() noexcept
{
    xaml_ptr<xaml_element_base> parent;
    XAML_RETURN_IF_FAILED(get_parent(&parent));
    if (parent)
    {
        xaml_win32_window_create_params params = {};
        params.class_name = WC_COMBOBOX;
        params.style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_AUTOHSCROLL | CBS_HASSTRINGS;
        params.x = 0;
        params.y = 0;
        params.width = 100;
        params.height = 50;
        params.parent = parent;
        if (m_is_editable)
        {
            params.style |= CBS_DROPDOWN;
        }
        else
        {
            params.style |= CBS_DROPDOWNLIST;
        }
        XAML_RETURN_IF_FAILED(create(params));
        XAML_RETURN_IF_FAILED(draw_items());
        XAML_RETURN_IF_FAILED(draw_sel());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_visible());
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::size_to_fit() noexcept
{
    double fw = 0.0, fh = 0.0;
    if (m_items)
    {
        xaml_codecvt_pool pool;
        XAML_FOREACH_START(item, m_items);
        {
            XAML_RETURN_IF_FAILED(create_item(item));
            xaml_ptr<xaml_string> s = item.query<xaml_string>();
            if (s)
            {
                wstring_view data;
                XAML_RETURN_IF_FAILED(pool(s, &data));
                xaml_size msize;
                XAML_RETURN_IF_FAILED(measure_string(data, { 5, 10 }, &msize));
                fw = (max)(fw, msize.width);
                fh = (max)(fh, msize.height);
            }
        }
        XAML_FOREACH_END();
    }
    return set_size_noevent({ fw, fh });
}

xaml_result xaml_combo_box_internal::insert_item(int32_t index, xaml_ptr<xaml_object> const& value) noexcept
{
    xaml_ptr<xaml_string> s = value.query<xaml_string>();
    if (s)
    {
        nowide::wstackstring data;
        XAML_RETURN_IF_FAILED(to_wstring(s, &data));
        ComboBox_InsertString(m_handle, index, data.c_str());
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::remove_item(int32_t index) noexcept
{
    ComboBox_DeleteString(m_handle, index);
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::clear_items() noexcept
{
    ComboBox_ResetContent(m_handle);
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::replace_item(int32_t index, xaml_ptr<xaml_object> const& value) noexcept
{
    xaml_ptr<xaml_string> s = value.query<xaml_string>();
    if (s)
    {
        nowide::wstackstring data;
        XAML_RETURN_IF_FAILED(to_wstring(s, &data));
        ComboBox_SetItemData(m_handle, index, data.c_str());
    }
    return XAML_S_OK;
}
