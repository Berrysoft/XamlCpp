#ifndef XAML_UI_CANVAS_D2D1_HPP
#define XAML_UI_CANVAS_D2D1_HPP

#include <d2d1.h>
#include <dwrite.h>
#include <wil/com.h>
#include <wil/resource.h>
#include <xaml/ui/controls/native_canvas.hpp>
#include <xaml/ui/native_drawing.hpp>

namespace xaml
{
    struct drawing_context_d2d : native_drawing_context
    {
        wil::com_ptr<ID2D1RenderTarget> target{ nullptr };
        wil::com_ptr<ID2D1Factory> d2d{ nullptr };
        wil::com_ptr<IDWriteFactory> dwrite{ nullptr };

        ~drawing_context_d2d() override;

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

    class canvas_d2d : public native_canvas
    {
    private:
        wil::com_ptr<ID2D1HwndRenderTarget> target{ nullptr };
        wil::com_ptr<ID2D1Factory> d2d{ nullptr };
        wil::com_ptr<IDWriteFactory> dwrite{ nullptr };

    public:
        canvas_d2d();
        ~canvas_d2d() override;

        bool create(HWND wnd) override;
        void begin_paint(HWND wnd, size real, std::function<void(drawing_context&)> paint_func) override;
    };

    inline size from_native(D2D1_SIZE_F s)
    {
        return { (double)s.width, (double)s.height };
    }
    template <>
    inline D2D1_SIZE_F to_native<D2D1_SIZE_F, size>(size s)
    {
        return { (float)s.width, (float)s.height };
    }

    inline point from_native(D2D1_POINT_2F p)
    {
        return { (double)p.x, (double)p.y };
    }
    template <>
    inline D2D1_POINT_2F to_native<D2D1_POINT_2F, point>(point p)
    {
        return { (float)p.x, (float)p.y };
    }

    inline rectangle from_native(D2D1_RECT_F r)
    {
        return { (double)r.left, (double)r.top, (double)(r.right - r.left), (double)(r.bottom - r.top) };
    }
    template <>
    inline D2D1_RECT_F to_native<D2D1_RECT_F, rectangle>(rectangle r)
    {
        return { (float)r.x, (float)r.y, (float)(r.x + r.width), (float)(r.y + r.height) };
    }
} // namespace xaml

#endif // !XAML_UI_CANVAS_D2D1_HPP
