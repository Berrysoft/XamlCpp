#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/label.hpp>

namespace xaml
{
    label::label() : common_control()
    {
        add_text_changed([this](label const&, string_view_t) {
            if (get_handle())
            {
                draw_text();
                __parent_redraw();
            }
        });
        add_size_changed([this](control const&, size) {
            if (get_handle())
            {
                draw_size();
                __parent_redraw();
            }
        });
    }

    label::~label() {}

    void label::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            window_create_params params = {};
            params.class_name = U("STATIC");
            params.style = SS_LEFT;
            params.x = 0;
            params.y = 0;
            params.width = 100;
            params.height = 50;
            params.parent = get_parent().get();
            this->__create(params);
        }
        rectangle real = region - get_margin();
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, real.x, real.y, real.width, real.height, SWP_NOZORDER));
        __set_size_noevent({ real.width, real.height });
        draw_text();
        draw_alignment();
        SetParent(get_handle(), get_parent()->get_handle());
        ShowWindow(get_handle(), SW_SHOW);
    }

    void label::draw_size()
    {
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, 0, 0, get_width(), get_height(), SWP_NOZORDER | SWP_NOMOVE));
    }

    void label::draw_text()
    {
        THROW_IF_WIN32_BOOL_FALSE(Static_SetText(get_handle(), m_text.c_str()));
    }

    void label::draw_alignment()
    {
        LONG_PTR style;
        switch (m_text_halignment)
        {
        case halignment_t::center:
            style = SS_CENTER;
            break;
        case halignment_t::right:
            style = SS_RIGHT;
            break;
        default:
            style = SS_LEFT;
            break;
        }
        SetWindowLongPtr(get_handle(), GWL_STYLE, style);
    }

    void label::__size_to_fit()
    {
        __set_size_noevent(__measure_text_size(m_text));
        draw_size();
    }
} // namespace xaml
