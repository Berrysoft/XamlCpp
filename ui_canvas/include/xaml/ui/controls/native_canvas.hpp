#ifndef XAML_UI_CONTROLS_NATIVE_CANVAS_HPP
#define XAML_UI_CONTROLS_NATIVE_CANVAS_HPP

#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/window.hpp>

#ifdef XAML_UI_WINDOWS
#elif defined(XAML_UI_GTK3)
#include <gtk/gtk.h>
#endif // XAML_UI_WINDOWS

namespace xaml
{
    //    struct native_drawing_context
    //    {
    //#ifdef XAML_UI_WINDOWS
    //#ifdef XAML_UI_CANVAS_DIRECT2D
    //        using native_handle_type = wil::com_ptr<ID2D1RenderTarget>;
    //#else
    //        using native_handle_type = Gdiplus::Graphics*;
    //#endif // XAML_UI_CANVAS_DIRECT2D
    //#elif defined(XAML_UI_GTK3)
    //        using native_handle_type = cairo_t*;
    //#elif defined(XAML_UI_COCOA)
    //        using native_handle_type = OBJC_OBJECT(NSGraphicsContext);
    //#endif
    //
    //        native_handle_type handle{ OBJC_NIL };
    //
    //#if defined(XAML_UI_WINDOWS) && defined(XAML_UI_CANVAS_DIRECT2D)
    //        using native_d2_type = wil::com_ptr<ID2D1Factory>;
    //        using native_dwrite_type = wil::com_ptr<IDWriteFactory>;
    //
    //        native_d2_type d2{ nullptr };
    //        native_dwrite_type dwrite{ nullptr };
    //#endif // XAML_UI_WINDOWS && XAML_UI_CANVAS_DIRECT2D
    //    };
    struct native_drawing_context
    {
        virtual ~native_drawing_context() {}
        virtual void draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle) = 0;
        virtual void fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle) = 0;
        virtual void draw_ellipse(drawing_pen const& pen, rectangle const& region) = 0;
        virtual void fill_ellipse(drawing_brush const& brush, rectangle const& region) = 0;
        virtual void draw_line(drawing_pen const& pen, point startp, point endp) = 0;
        virtual void draw_rect(drawing_pen const& pen, rectangle const& rect) = 0;
        virtual void fill_rect(drawing_brush const& brush, rectangle const& rect) = 0;
        virtual void draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round) = 0;
        virtual void fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round) = 0;
        virtual void draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str) = 0;
    };

#ifdef XAML_UI_WINDOWS
    struct native_canvas
    {
        //#endif // XAML_UI_CANVAS_DIRECT2D
        virtual ~native_canvas() {}
        virtual bool create(std::shared_ptr<window> wnd, rectangle const& real) = 0;
        virtual void begin_paint(std::shared_ptr<window> wnd, rectangle const& real, std::function<void(drawing_context&)> paint_func) = 0;
    };
#endif // XAML_UI_WINDOWS

} // namespace xaml

#endif // !XAML_UI_CONTROLS_NATIVE_CANVAS_HPP
