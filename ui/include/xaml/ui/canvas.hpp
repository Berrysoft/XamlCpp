#ifndef XAML_UI_CANVAS_HPP
#define XAML_UI_CANVAS_HPP

#include <xaml/ui/control.hpp>
#include <xaml/ui/drawing.hpp>

#ifdef XAML_UI_WINDOWS
#include <gdiplus.h>
#include <memory>
#include <wil/resource.h>
#endif // XAML_UI_WINDOWS

namespace xaml
{
#ifdef XAML_UI_GTK3
    struct __cairo_fill_t
    {
        color fill;
    };

    struct __cairo_stroke_t
    {
        color stroke;
        double width;
    };
#endif

    class drawing_brush
    {
    public:
#ifdef XAML_UI_WINDOWS
        using native_object_type = Gdiplus::SolidBrush;
#else
        using native_object_type = __cairo_fill_t;
#endif // XAML_UI_WINDOWS
        using native_handle_type = native_object_type const*;

    private:
        native_object_type m_object;

    public:
        drawing_brush(color c);

        color get_color() const;
        void set_color(color value);

        constexpr native_handle_type get_handle() const noexcept { return &m_object; }
    };

    class drawing_pen
    {
    public:
#ifdef XAML_UI_WINDOWS
        using native_object_type = Gdiplus::Pen;
#else
        using native_object_type = __cairo_stroke_t;
#endif // XAML_UI_WINDOWS
        using native_handle_type = native_object_type const*;

    private:
        native_object_type m_object;

    public:
        drawing_pen(color c, double width = 1.0);

        color get_color() const;
        void set_color(color value);

        double get_width() const;
        void set_width(double value);

        constexpr native_handle_type get_handle() const noexcept { return &m_object; }
    };

    class drawing_context
    {
    public:
#ifdef XAML_UI_WINDOWS
        using native_handle_type = Gdiplus::Graphics*;
#elif defined(XAML_UI_GTK3)
        using native_handle_type = cairo_t*;
#endif // XAML_UI_WINDOWS

    private:
        native_handle_type m_handle;

    public:
        constexpr native_handle_type get_handle() const noexcept { return m_handle; }

        drawing_context(native_handle_type handle);

#ifdef XAML_UI_GTK3
    private:
        void set_pen(drawing_pen const& pen);
        void set_brush(drawing_brush const& brush);
        void path_arc(rectangle const& region, double start_angle, double end_angle);
        void path_rect(rectangle const& rect);
        void path_round_rect(rectangle const& rect, size round);
#endif // XAML_UI_GTK3

    public:
        void draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle);
        void fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle);
        void draw_ellipse(drawing_pen const& pen, rectangle const& region);
        void fill_ellipse(drawing_brush const& brush, rectangle const& region);
        void draw_line(drawing_pen const& pen, point startp, point endp);
        void draw_rect(drawing_pen const& pen, rectangle const& rect);
        void fill_rect(drawing_brush const& brush, rectangle const& rect);
        void draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round);
        void fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round);
        void draw_string(drawing_brush const& brush, point p, string_view_t str);
    };

    class canvas : public common_control, public meta_class_impl<canvas>
    {
    private:
        static constexpr std::string_view namespace_name = "xaml";
        static constexpr std::string_view class_name = "canvas";

    public:
        canvas();
        ~canvas() override;

    private:
        rectangle m_real_region;

#ifdef XAML_UI_WINDOWS
    private:
        wil::unique_hdc_window m_store_dc;

    public:
        virtual std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    private:
        static gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data);
#endif // XAML_UI_GTK3

    public:
        EVENT(redraw, canvas const&, drawing_context&)

        void __draw(rectangle const& region) override;

        constexpr size get_size() const noexcept { return { m_real_region.width, m_real_region.height }; }

        constexpr double get_width() const noexcept { return m_real_region.width; }

        constexpr double get_height() const noexcept { return m_real_region.height; }

#define ADD_CANVAS_MEMBERS() \
    ADD_COMMON_CONTROL_MEMBERS()

        static void register_class() noexcept
        {
            REGISTER_TYPE();
            ADD_CTOR_DEF();
            ADD_CANVAS_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_CANVAS_HPP
