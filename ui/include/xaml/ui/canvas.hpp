#ifndef XAML_UI_CANVAS_HPP
#define XAML_UI_CANVAS_HPP

#include <xaml/ui/control.hpp>
#include <xaml/ui/drawing.hpp>

#ifdef XAML_UI_WINDOWS
#include <gdiplus.h>
#include <memory>
#include <wil/resource.h>
#elif defined(XAML_UI_COCOA)
#include <xaml/ui/objc.hpp>
#endif // XAML_UI_WINDOWS

namespace xaml
{
#if defined(XAML_UI_GTK3) || defined(XAML_UI_COCOA)
    struct __drawing_fill_t
    {
        color fill;
    };

    struct __drawing_stroke_t
    {
        color stroke;
        double width;
    };

    struct __drawing_font_t
    {
        string_t font_family;
        double size;
        bool italic;
        bool bold;
    };
#endif

    class drawing_brush
    {
    public:
#ifdef XAML_UI_WINDOWS
        using native_object_type = Gdiplus::SolidBrush;
#elif defined(XAML_UI_GTK3) || defined(XAML_UI_COCOA)
        using native_object_type = __drawing_fill_t;
#endif
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
#elif defined(XAML_UI_GTK3) || defined(XAML_UI_COCOA)
        using native_object_type = __drawing_stroke_t;
#endif
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

    class drawing_font
    {
    public:
#ifdef XAML_UI_WINDOWS
        using native_object_type = Gdiplus::Font;
#elif defined(XAML_UI_GTK3) || defined(XAML_UI_COCOA)
        using native_object_type = __drawing_font_t;
#endif
        using native_handle_type = native_object_type const*;

    private:
        native_object_type m_object;

    public:
        drawing_font(string_view_t family, double size, bool italic, bool bold);

        string_view_t get_font_family() const;
        void set_font_family(string_view_t value);

        double get_size() const;

        bool get_italic() const;

        bool get_bold() const;

        constexpr native_handle_type get_handle() const noexcept { return &m_object; }
    };

    class drawing_context
    {
    public:
#ifdef XAML_UI_WINDOWS
        using native_handle_type = Gdiplus::Graphics*;
#elif defined(XAML_UI_GTK3)
        using native_handle_type = cairo_t*;
#elif defined(XAML_UI_COCOA)
        using native_handle_type = OBJC_OBJECT(NSGraphicsContext);
#endif

    private:
        native_handle_type m_handle;

    public:
        inline native_handle_type get_handle() const noexcept { return m_handle; }

        drawing_context(native_handle_type handle);

#ifdef XAML_UI_COCOA
    private:
        size m_size;

    public:
        constexpr size __get_size() const noexcept { return m_size; }
        void __set_size(size value) { m_size = value; }
#endif // XAML_UI_COCOA

#ifdef XAML_UI_GTK3
    private:
        void set_pen(drawing_pen const& pen);
        void set_brush(drawing_brush const& brush);
        void path_arc(rectangle const& region, double start_angle, double end_angle);
        void path_rect(rectangle const& rect);
        void path_round_rect(rectangle const& rect, size round);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    private:
        using path_type = OBJC_OBJECT(NSBezierPath);

        path_type path_ellipse(rectangle const& region);
        path_type path_rect(rectangle const& rect);
        path_type path_round_rect(rectangle const& rect, size round);
#endif // XAML_UI_COCOA

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
        void draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str);
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

    protected:
        size __get_compact_size() const override { return get_size(); }

    public:
        virtual std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    private:
        static gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    public:
        void __on_draw_rect();
#endif // XAML_UI_COCOA

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
