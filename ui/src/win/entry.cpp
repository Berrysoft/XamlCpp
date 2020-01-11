#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/entry.hpp>

namespace xaml
{
    XAML_API void entry::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            window_create_params params = {};
            params.class_name = U("EDIT");
            params.style = WS_CHILD | WS_VISIBLE | ES_LEFT;
            params.ex_style = WS_EX_CLIENTEDGE;
            params.x = 0;
            params.y = 0;
            params.width = 100;
            params.height = 50;
            params.parent = get_parent().get();
            this->__create(params);
        }
        rectangle real = region - get_margin();
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, (int)real.x, (int)real.y, (int)real.width, (int)real.height, SWP_NOZORDER));
        __set_size_noevent({ real.width, real.height });
        draw_text();
        draw_alignment();
        SetParent(get_handle(), get_parent()->get_handle());
        ShowWindow(get_handle(), SW_SHOW);
    }

    XAML_API void entry::draw_size()
    {
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, 0, 0, (int)get_width(), (int)get_height(), SWP_NOZORDER | SWP_NOMOVE));
    }

    XAML_API void entry::draw_text()
    {
        THROW_IF_WIN32_BOOL_FALSE(Edit_SetText(get_handle(), m_text.c_str()));
    }

    XAML_API void entry::draw_alignment()
    {
        LONG_PTR style;
        switch (m_text_halignment)
        {
        case halignment_t::center:
            style = ES_CENTER;
            break;
        case halignment_t::right:
            style = ES_RIGHT;
            break;
        default:
            style = ES_LEFT;
            break;
        }
        SetWindowLongPtr(get_handle(), GWL_STYLE, style);
    }

    XAML_API void entry::__size_to_fit()
    {
        size msize = __measure_text_size(m_text);
        __set_size_noevent({ msize.width + 15, msize.height + 15 });
        draw_size();
    }
} // namespace xaml
