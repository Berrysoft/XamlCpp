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
        case WM_PAINT:
        {
            if (auto parent = __get_window(msg.hWnd))
            {
                UINT udpi = GetDpiForWindow(get_handle()->handle);
                rectangle region = m_real_region * udpi / 96.0;
                HDC dc = parent->get_window()->store_dc.get();
                wil::unique_hfont oldf{ SelectFont(dc, application::current()->__default_font(get_handle()->handle)) };
                SIZE s = {};
                THROW_IF_WIN32_BOOL_FALSE(GetTextExtentPoint32(dc, m_text.c_str(), (int)m_text.length(), &s));
                size text_size = from_native(s);
                double real_x = region.x;
                switch (m_text_halignment)
                {
                case halignment_t::center:
                    real_x = region.x + region.width / 2 - text_size.width / 2;
                    break;
                case halignment_t::right:
                    real_x = region.x + region.width - text_size.width;
                    break;
                }
                TextOut(dc, (int)real_x, (int)region.y, m_text.c_str(), (int)m_text.length());
                DeleteFont(SelectFont(dc, oldf.release()));
            }
            break;
        }
        }
        return nullopt;
    }

    void label::__draw(rectangle const& region)
    {
        if (auto sparent = get_parent().lock())
        {
            set_handle(get_parent().lock()->get_handle());
            rectangle real = region - get_margin();
            m_real_region = real;
            __set_size_noevent({ real.width, real.height });
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
