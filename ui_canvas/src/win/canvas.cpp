#include <win/canvas_gdiplus.hpp>
#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/controls/native_canvas.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_window.hpp>
#include <xaml/ui/window.hpp>

#ifdef XAML_UI_CANVAS_DIRECT2D
#include <win/canvas_d2d.hpp>
#endif // XAML_UI_CANVAS_DIRECT2D

using namespace std;

namespace xaml
{
    canvas::canvas() : control() {}

    canvas::~canvas() {}

    optional<std::intptr_t> canvas::__wnd_proc(window_message const& msg)
    {
        if (get_handle() && msg.hWnd == get_handle()->handle)
        {
            switch (msg.Msg)
            {
            case WM_PAINT:
            {
                if (auto wnd = __get_window(get_handle()->handle))
                {
                    m_canvas->begin_paint(wnd, m_real_region, [this](drawing_context& dc) { m_redraw(*this, dc); });
                }
                break;
            }
            }
        }
        return nullopt;
    }

    void canvas::__draw(rectangle const& region)
    {
        if (auto sparent = get_parent().lock())
        {
            set_handle(sparent->get_handle());
            auto real = region - get_margin();
            if (m_real_region != real)
            {
                m_real_region = real;
                __set_size_noevent({ real.width, real.height });
                if (!get_canvas())
                {
#ifdef XAML_UI_CANVAS_DIRECT2D
                    set_canvas(make_shared<canvas_d2d>());
                    if (!get_canvas()->create(nullptr, m_real_region))
                    {
                        set_canvas(make_shared<canvas_gdiplus>());
                    }
#else
                    set_canvas(make_shared<canvas_gdiplus>());
#endif // XAML_UI_CANVAS_DIRECT2D
                }
                if (auto wnd = __get_window(get_handle()->handle))
                {
                    get_canvas()->create(wnd, m_real_region);
                }
            }
        }
    }
} // namespace xaml
