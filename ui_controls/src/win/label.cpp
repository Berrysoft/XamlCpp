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
    //static wil::unique_hbrush white_brush{ CreateSolidBrush(RGB(255, 255, 255)) };

    optional<std::intptr_t> label::__wnd_proc(window_message const& msg)
    {
        switch (msg.Msg)
        {
        //case WM_CTLCOLORSTATIC:
        //{
        //    return (std::intptr_t)white_brush.get();
        //}
        case WM_PAINT:
        {
            if (auto parent = __get_window(msg.hWnd))
            {
                UINT udpi = GetDpiForWindow(get_handle()->handle);
                rectangle region = m_real_region * udpi / 96.0;
                HDC dc = parent->get_window()->store_dc.get();
                SelectFont(dc, application::current()->__default_font());
                SIZE s = {};
                THROW_IF_WIN32_BOOL_FALSE(GetTextExtentPoint32(dc, m_text.c_str(), (int)m_text.length(), &s));
                size text_size = from_native(s) * udpi / 96.0;
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
            //if (!get_handle())
            //{
            //    window_create_params params = {};
            //    params.class_name = WC_STATIC;
            //    params.style = WS_TABSTOP | WS_CHILD | WS_VISIBLE | SS_LEFT;
            //    params.x = 0;
            //    params.y = 0;
            //    params.width = 100;
            //    params.height = 50;
            //    params.parent = sparent.get();
            //    this->__create(params);
            //    draw_text();
            //    draw_alignment();
            //    SetParent(get_handle()->handle, sparent->get_handle()->handle);
            //    ShowWindow(get_handle()->handle, SW_SHOW);
            //}
            set_handle(get_parent().lock()->get_handle());
            rectangle real = region - get_margin();
            m_real_region = real;
            //UINT udpi = GetDpiForWindow(get_handle()->handle);
            //rectangle real_real = real * udpi / 96.0;
            //THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle()->handle, HWND_TOP, (int)real_real.x, (int)real_real.y, (int)real_real.width, (int)real_real.height, SWP_NOZORDER));
            __set_size_noevent({ real.width, real.height });
        }
    }

    void label::draw_text()
    {
        //THROW_IF_WIN32_BOOL_FALSE(Static_SetText(get_handle()->handle, m_text.c_str()));
    }

    void label::draw_alignment()
    {
        //LONG_PTR style = WS_CHILD | WS_VISIBLE;
        //switch (m_text_halignment)
        //{
        //case halignment_t::center:
        //    style |= SS_CENTER;
        //    break;
        //case halignment_t::right:
        //    style |= SS_RIGHT;
        //    break;
        //default:
        //    style |= SS_LEFT;
        //    break;
        //}
        //SetWindowLongPtr(get_handle()->handle, GWL_STYLE, style);
    }

    void label::__size_to_fit()
    {
        __set_size_noevent(__measure_text_size(m_text, {}));
    }
} // namespace xaml
