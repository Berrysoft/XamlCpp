#include <algorithm>
#include <shared/combo_box.hpp>
#include <windowsx.h>
#include <xaml/ui/controls/combo_box.h>

#include <CommCtrl.h>

using namespace std;

xaml_result xaml_combo_box_impl::wnd_proc(xaml_win32_window_message const& msg, LRESULT* presult) noexcept
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
                xaml_std_string_t text(count, U('\0'));
                ComboBox_GetText(m_handle, text.data(), count + 1);
                xaml_ptr<xaml_string> s;
                XAML_RETURN_IF_FAILED(xaml_string_new(move(text), &s));
                XAML_RETURN_IF_FAILED(set_text(s.get()));
                break;
            }
            }
        }
    }
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_impl::draw(xaml_rectangle const& region) noexcept
{
    if (m_parent)
    {
        if (!m_handle)
        {
            XAML_RETURN_IF_FAILED(draw_editable());
        }
        XAML_RETURN_IF_FAILED(set_rect(region));
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_impl::draw_text() noexcept
{
    if (m_text)
    {
        xaml_char_t const* data;
        XAML_RETURN_IF_FAILED(m_text->get_data(&data));
        XAML_RETURN_IF_WIN32_BOOL_FALSE(ComboBox_SetText(m_handle, data));
    }
    else
    {
        XAML_RETURN_IF_WIN32_BOOL_FALSE(ComboBox_SetText(m_handle, nullptr));
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_impl::draw_items() noexcept
{
    XAML_FOREACH_START(item, m_items);
    {
        xaml_ptr<xaml_string> s = item.query<xaml_string>();
        if (s)
        {
            xaml_char_t const* data;
            XAML_RETURN_IF_FAILED(s->get_data(&data));
            ComboBox_AddString(m_handle, data);
        }
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

xaml_result xaml_combo_box_impl::draw_sel() noexcept
{
    ComboBox_SetCurSel(m_handle, m_sel_id);
    return XAML_S_OK;
}

xaml_result xaml_combo_box_impl::draw_editable() noexcept
{
    if (m_parent)
    {
        xaml_win32_window_create_params params = {};
        params.class_name = WC_COMBOBOX;
        params.style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_AUTOHSCROLL | CBS_HASSTRINGS;
        params.x = 0;
        params.y = 0;
        params.width = 100;
        params.height = 50;
        params.parent = m_parent;
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

xaml_result xaml_combo_box_impl::size_to_fit() noexcept
{
    double fw = 0.0, fh = 0.0;
    XAML_FOREACH_START(item, m_items);
    {
        xaml_ptr<xaml_string> s = item.query<xaml_string>();
        if (s)
        {
            xaml_size msize;
            XAML_RETURN_IF_FAILED(measure_string(s, { 5, 5 }, &msize));
            fw = (max)(fw, msize.width);
            fh = (max)(fh, msize.height);
        }
    }
    XAML_FOREACH_END();
    return set_size_noevent({ fw, fh });
}

xaml_result xaml_combo_box_impl::insert_item(int32_t index, xaml_ptr<xaml_object> const& value) noexcept
{
    xaml_ptr<xaml_string> s = value.query<xaml_string>();
    if (s)
    {
        xaml_char_t const* data;
        XAML_RETURN_IF_FAILED(s->get_data(&data));
        ComboBox_InsertString(m_handle, index, data);
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_impl::remove_item(int32_t index) noexcept
{
    ComboBox_DeleteString(m_handle, index);
    return XAML_S_OK;
}

xaml_result xaml_combo_box_impl::clear_items() noexcept
{
    ComboBox_ResetContent(m_handle);
    return XAML_S_OK;
}

xaml_result xaml_combo_box_impl::replace_item(int32_t index, xaml_ptr<xaml_object> const& value) noexcept
{
    xaml_ptr<xaml_string> s = value.query<xaml_string>();
    if (s)
    {
        xaml_char_t const* data;
        XAML_RETURN_IF_FAILED(s->get_data(&data));
        ComboBox_SetItemData(m_handle, index, data);
    }
    return XAML_S_OK;
}
