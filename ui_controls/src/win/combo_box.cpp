#include <algorithm>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/controls/combo_box.hpp>

#include <CommCtrl.h>

using namespace std;

namespace xaml
{
    void combo_box::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            draw_create();
        }
        rectangle real = region - get_margin();
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, (int)real.x, (int)real.y, (int)real.width, (int)real.height, SWP_NOZORDER));
        __set_size_noevent({ real.width, real.height });
        draw_sel();
        draw_editable();
        SetParent(get_handle(), get_parent()->get_handle());
    }

    void combo_box::draw_create()
    {
        window_create_params params = {};
        params.class_name = WC_COMBOBOX;
        params.style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_AUTOHSCROLL;
        params.x = 0;
        params.y = 0;
        params.width = 100;
        params.height = 50;
        params.parent = get_parent().get();
        if (m_is_editable)
        {
            params.style |= CBS_DROPDOWN;
        }
        else
        {
            params.style |= CBS_DROPDOWNLIST;
        }
        this->__create(params);
        draw_items();
    }

    void combo_box::draw_size()
    {
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, 0, 0, (int)get_width(), (int)get_height(), SWP_NOZORDER | SWP_NOMOVE));
    }

    void combo_box::draw_text()
    {
        if (m_text)
        {
            ComboBox_SetText(get_handle(), m_text->c_str());
        }
        else
        {
            ComboBox_SetText(get_handle(), nullptr);
        }
    }

    void combo_box::draw_items()
    {
        for (string_t const& item : m_items)
        {
            ComboBox_AddString(get_handle(), item.c_str());
        }
    }

    void combo_box::draw_sel()
    {
        ComboBox_SetCurSel(get_handle(), m_sel_id);
    }

    void combo_box::draw_editable()
    {
    }

    void combo_box::__size_to_fit()
    {
        double fw = 0.0, fh = 0.0;
        for (string_t const& item : m_items)
        {
            size msize = __measure_text_size(item);
            fw = (max)(fw, msize.width);
            fh = (max)(fh, msize.height);
        }
        __set_size_noevent({ fw + 20, fh + 15 });
        draw_size();
    }
} // namespace xaml
