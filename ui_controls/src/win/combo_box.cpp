#include <algorithm>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/controls/combo_box.hpp>
#include <xaml/ui/native_control.hpp>

#include <CommCtrl.h>

using namespace std;

namespace xaml
{
    optional<std::intptr_t> combo_box::__wnd_proc(window_message const& msg)
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
                case CBN_SELCHANGE:
                {
                    set_sel_id(ComboBox_GetCurSel(get_handle()->handle));
                    int count = ComboBox_GetTextLength(get_handle()->handle);
                    string_t text(count, U('\0'));
                    ComboBox_GetText(get_handle()->handle, text.data(), count);
                    set_text(text);
                    break;
                }
                }
            }
        }
        }
        return nullopt;
    }

    void combo_box::__draw(rectangle const& region)
    {
        if (auto sparent = get_parent().lock())
        {
            if (!get_handle())
            {
                draw_editable();
                draw_items();
                draw_sel();
                draw_text();
                SetParent(get_handle()->handle, sparent->get_handle()->handle);
            }
            __set_rect(region);
        }
    }

    void combo_box::draw_text()
    {
        if (m_text)
        {
            ComboBox_SetText(get_handle()->handle, m_text->c_str());
        }
        else
        {
            ComboBox_SetText(get_handle()->handle, nullptr);
        }
    }

    void combo_box::draw_items()
    {
        for (auto item : get_items())
        {
            ComboBox_AddString(get_handle()->handle, item->c_str());
        }
    }

    void combo_box::draw_sel()
    {
        ComboBox_SetCurSel(get_handle()->handle, get_sel_id());
    }

    void combo_box::draw_editable()
    {
        if (auto sparent = get_parent().lock())
        {
            window_create_params params = {};
            params.class_name = WC_COMBOBOX;
            params.style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_AUTOHSCROLL | CBS_HASSTRINGS;
            params.x = 0;
            params.y = 0;
            params.width = 100;
            params.height = 50;
            params.parent = sparent.get();
            if (m_is_editable)
            {
                params.style |= CBS_DROPDOWN;
            }
            else
            {
                params.style |= CBS_DROPDOWNLIST;
            }
            this->__create(params);
        }
    }

    void combo_box::__size_to_fit()
    {
        double fw = 0.0, fh = 0.0;
        for (auto item : get_items())
        {
            size msize = __measure_text_size(*item, { 2, 5 });
            fw = (max)(fw, msize.width);
            fh = (max)(fh, msize.height);
        }
        __set_size_noevent({ fw, fh });
    }

    void combo_box::insert_item(size_t index, string_t const& value)
    {
        ComboBox_InsertString(get_handle()->handle, index, value.c_str());
    }

    void combo_box::remove_item(size_t index)
    {
        ComboBox_DeleteString(get_handle()->handle, index);
    }

    void combo_box::clear_items()
    {
        ComboBox_ResetContent(get_handle()->handle);
    }

    void combo_box::replace_item(size_t index, string_t const& value)
    {
        ComboBox_SetItemData(get_handle()->handle, index, value.c_str());
    }
} // namespace xaml
