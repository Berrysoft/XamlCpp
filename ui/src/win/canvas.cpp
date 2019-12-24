#include <Windows.h>
#include <cmath>
#include <wil/result_macros.h>
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
} // namespace xaml
