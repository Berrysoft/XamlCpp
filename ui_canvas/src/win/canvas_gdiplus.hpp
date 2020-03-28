#ifndef XAML_UI_CANVAS_GDIPLUS_HPP
#define XAML_UI_CANVAS_GDIPLUS_HPP

#include <Windows.h>
#include <optional>
#include <wil/resource.h>
#include <xaml/ui/controls/native_canvas.hpp>
#include <xaml/ui/native_drawing.hpp>

#include <gdiplus.h>

namespace xaml
{
    struct drawing_context_gdiplus : native_drawing_context
    {
        double dpi{ 96.0 };
        Gdiplus::Graphics* handle{ nullptr };

        ~drawing_context_gdiplus() override;

        void draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle) override;
        void fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle) override;
        void draw_ellipse(drawing_pen const& pen, rectangle const& region) override;
        void fill_ellipse(drawing_brush const& brush, rectangle const& region) override;
        void draw_line(drawing_pen const& pen, point startp, point endp) override;
        void draw_rect(drawing_pen const& pen, rectangle const& rect) override;
        void fill_rect(drawing_brush const& brush, rectangle const& rect) override;
        void draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round) override;
        void fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round) override;
        void draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str) override;
    };

    class canvas_gdiplus : public native_canvas
    {
    private:
        std::optional<ULONG_PTR> m_token{};

    public:
        canvas_gdiplus();
        ~canvas_gdiplus() override;

        bool create(HWND wnd) override;
        void begin_paint(HWND wnd, size real, std::function<void(std::shared_ptr<drawing_context>)> paint_func) override;
    };

    inline size from_native(Gdiplus::SizeF s)
    {
        return { (double)s.Width, (double)s.Height };
    }
    template <>
    inline Gdiplus::SizeF to_native<Gdiplus::SizeF, size>(size s)
    {
        return { (float)s.width, (float)s.height };
    }

    inline point from_native(Gdiplus::PointF p)
    {
        return { (double)p.X, (double)p.Y };
    }
    template <>
    inline Gdiplus::PointF to_native<Gdiplus::PointF, point>(point p)
    {
        return { (float)p.x, (float)p.y };
    }

    inline rectangle from_native(Gdiplus::RectF r)
    {
        return { (double)r.X, (double)r.Y, (double)r.Width, (double)r.Height };
    }
    template <>
    inline Gdiplus::RectF to_native<Gdiplus::RectF, rectangle>(rectangle r)
    {
        return { (float)r.x, (float)r.y, (float)r.width, (float)r.height };
    }
} // namespace xaml

#endif // !XAML_UI_CANVAS_GDIPLUS_HPP
