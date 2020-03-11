#include <wil/resource.h>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/controls/label.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>
#include <xaml/ui/native_window.hpp>
#include <xaml/ui/window.hpp>

#include <CommCtrl.h>

using namespace std;

namespace xaml
{
    optional<std::intptr_t> label::__wnd_proc(window_message const& msg)
    {
        switch (msg.Msg)
        {
        case WM_DRAWITEM:
        {
            DRAWITEMSTRUCT* ds = (DRAWITEMSTRUCT*)msg.lParam;
            if (ds->hwndItem == get_handle()->handle)
            {
                size region = __get_real_size();
                HDC dc = ds->hDC;
                THROW_IF_WIN32_BOOL_FALSE(Rectangle(dc, -1, -1, (int)region.width + 2, (int)region.height + 2));
                wil::unique_hfont oldf{ SelectFont(dc, application::current()->__default_font(get_handle()->handle)) };
                SIZE s = {};
                THROW_IF_WIN32_BOOL_FALSE(GetTextExtentPoint32(dc, m_text.c_str(), (int)m_text.length(), &s));
                size text_size = from_native(s);
                double real_x = 0;
                switch (m_text_halignment)
                {
                case halignment_t::center:
                    real_x += region.width / 2 - text_size.width / 2;
                    break;
                case halignment_t::right:
                    real_x += region.width - text_size.width;
                    break;
                }
                THROW_IF_WIN32_BOOL_FALSE(TextOut(dc, (int)real_x, 0, m_text.c_str(), (int)m_text.length()));
                SelectFont(dc, oldf.release());
            }
            return TRUE;
        }
        }
        return nullopt;
    }

    void label::__draw(rectangle const& region)
    {
        if (auto sparent = get_parent().lock())
        {
            if (!get_handle())
            {
                window_create_params params = {};
                params.class_name = WC_STATIC;
                params.style = WS_CHILD | WS_VISIBLE | SS_OWNERDRAW;
                params.x = 0;
                params.y = 0;
                params.width = 100;
                params.height = 50;
                params.parent = sparent.get();
                this->__create(params);
                draw_text();
                draw_alignment();
                SetParent(get_handle()->handle, sparent->get_handle()->handle);
            }
            __set_rect(region);
        }
    }

    void label::draw_text()
    {
    }

    void label::draw_alignment()
    {
    }

    void label::__size_to_fit()
    {
        __set_size_noevent(__measure_text_size(m_text, {}));
    }
} // namespace xaml
