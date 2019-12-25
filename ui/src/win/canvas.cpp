#include <Windows.h>
#include <cmath>
#include <internal/win/drawing.hpp>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/canvas.hpp>

using namespace std;

namespace xaml
{
    void drawing_context::draw_arc(rectangle const& region, double start_angle, double end_angle)
    {
        double a = region.width / 2;
        double b = region.height / 2;
        double ct1 = cos(start_angle);
        double st1 = sin(start_angle);
        double ct2 = cos(end_angle);
        double st2 = sin(end_angle);
        double r1 = a * b / sqrt(b * b * ct1 * ct1 + a * a * st1 * st1);
        double r2 = a * b / sqrt(b * b * ct2 * ct2 + a * a * st2 * st2);
        double cpx = region.x + region.width / 2;
        double cpy = region.y + region.height / 2;
        THROW_IF_WIN32_BOOL_FALSE(Arc(m_handle, region.x, region.y, region.x + region.width, region.y + region.height, cpx + r1 * ct1, cpy + r1 * st1, cpx + r2 * ct2, cpy + r2 * st2));
    }

    void drawing_context::draw_ellipse(rectangle const& region)
    {
        THROW_IF_WIN32_BOOL_FALSE(Ellipse(m_handle, region.x, region.y, region.x + region.width, region.y + region.height));
    }

    void drawing_context::draw_line(point startp, point endp)
    {
        THROW_IF_WIN32_BOOL_FALSE(MoveToEx(m_handle, startp.x, startp.y, NULL));
        THROW_IF_WIN32_BOOL_FALSE(LineTo(m_handle, endp.x, endp.y));
    }

    void drawing_context::draw_rect(rectangle const& rect)
    {
        THROW_IF_WIN32_BOOL_FALSE(Rectangle(m_handle, rect.x, rect.y, rect.x + rect.width, rect.y + rect.height));
    }

    void drawing_context::draw_round_rect(rectangle const& rect, size round)
    {
        THROW_IF_WIN32_BOOL_FALSE(RoundRect(m_handle, rect.x, rect.y, rect.x + rect.width, rect.y + rect.height, round.width, round.height));
    }

    void drawing_context::draw_string(point p, string_view_t str)
    {
        THROW_IF_WIN32_BOOL_FALSE(TextOut(m_handle, p.x, p.y, str.data(), str.size()));
    }

    canvas::canvas() : common_control() {}

    canvas::~canvas() {}

    optional<LRESULT> canvas::__wnd_proc(window_message const& msg)
    {
        if (get_handle() && msg.hWnd == get_handle())
        {
            switch (msg.Msg)
            {
            case WM_PAINT:
            {
                PAINTSTRUCT ps;
                auto hDC = wil::BeginPaint(get_handle(), &ps);
                drawing_context dc{ m_store_dc.get() };
                m_redraw(*this, dc);
                BitBlt(hDC.get(), m_real_region.x, m_real_region.y, m_real_region.width, m_real_region.height, m_store_dc.get(), 0, 0, SRCCOPY);
            }
            }
        }
        return nullopt;
    }

    void canvas::__draw(rectangle const& region)
    {
        set_handle(get_parent()->get_handle());
        m_real_region = region - get_margin();
        auto wnd_dc = wil::GetDC(get_handle());
        m_store_dc.reset(CreateCompatibleDC(wnd_dc.get()));
        wil::unique_hbitmap bitmap{ CreateCompatibleBitmap(wnd_dc.get(), m_real_region.width, m_real_region.height) };
        wil::unique_hbitmap ori_bitmap{ SelectBitmap(m_store_dc.get(), bitmap.release()) };
        THROW_IF_WIN32_BOOL_FALSE(InvalidateRect(get_handle(), nullptr, TRUE));
    }
} // namespace xaml
