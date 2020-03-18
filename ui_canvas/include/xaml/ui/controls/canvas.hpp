#ifndef XAML_UI_CANVAS_HPP
#define XAML_UI_CANVAS_HPP

#include <xaml/ui/control.hpp>
#include <xaml/ui/drawing.hpp>

namespace xaml
{
    struct drawing_brush
    {
        color fill;
    };

    struct drawing_pen
    {
        color stroke;
        double width = 1.0;
    };

    struct drawing_font
    {
        string_t font_family;
        double size;
        bool italic;
        bool bold;
        halignment_t halign;
        valignment_t valign;
    };

    struct native_drawing_context;

    class drawing_context
    {
    public:
        using native_handle_type = native_drawing_context*;

    private:
        native_handle_type m_handle{ nullptr };

    public:
        inline native_handle_type get_handle() const noexcept { return m_handle; }

        XAML_UI_CANVAS_API drawing_context(native_handle_type handle);

#ifdef XAML_UI_GTK3
        void set_pen(drawing_pen const& pen);
        void set_brush(drawing_brush const& brush);
        void path_arc(rectangle const& region, double start_angle, double end_angle);
        void path_rect(rectangle const& rect);
        void path_round_rect(rectangle const& rect, size round);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    private:
        size m_size;

    public:
        constexpr size __get_size() const noexcept { return m_size; }
        void __set_size(size value) noexcept { m_size = value; }
#endif // XAML_UI_COCOA

    public:
        XAML_UI_CANVAS_API void draw_arc(drawing_pen const& pen, rectangle const& region, double start_angle, double end_angle);
        XAML_UI_CANVAS_API void fill_pie(drawing_brush const& brush, rectangle const& region, double start_angle, double end_angle);
        XAML_UI_CANVAS_API void draw_ellipse(drawing_pen const& pen, rectangle const& region);
        XAML_UI_CANVAS_API void fill_ellipse(drawing_brush const& brush, rectangle const& region);
        XAML_UI_CANVAS_API void draw_line(drawing_pen const& pen, point startp, point endp);
        XAML_UI_CANVAS_API void draw_rect(drawing_pen const& pen, rectangle const& rect);
        XAML_UI_CANVAS_API void fill_rect(drawing_brush const& brush, rectangle const& rect);
        XAML_UI_CANVAS_API void draw_round_rect(drawing_pen const& pen, rectangle const& rect, size round);
        XAML_UI_CANVAS_API void fill_round_rect(drawing_brush const& brush, rectangle const& rect, size round);
        XAML_UI_CANVAS_API void draw_string(drawing_brush const& brush, drawing_font const& font, point p, string_view_t str);
    };

    struct native_canvas;

    class canvas : public control
    {
    public:
        XAML_UI_CANVAS_API canvas();
        XAML_UI_CANVAS_API ~canvas() override;

#ifdef XAML_UI_WINDOWS
    public:
        using native_canvas_type = std::shared_ptr<native_canvas>;

    private:
        native_canvas_type m_canvas;

    public:
        native_canvas_type get_canvas() const noexcept { return m_canvas; }

    protected:
        void set_canvas(native_canvas_type value) noexcept { m_canvas = value; }

    public:
        XAML_UI_CANVAS_API virtual std::optional<std::intptr_t> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    private:
        static int on_draw(void* widget, void* cr, void* data);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    public:
        void __on_draw_rect();
#endif // XAML_UI_COCOA

    public:
        EVENT(redraw, std::reference_wrapper<canvas>, std::reference_wrapper<drawing_context>)

        XAML_UI_CANVAS_API void __draw(rectangle const& region) override;

    public:
#define ADD_CANVAS_MEMBERS() \
    ADD_CONTROL_MEMBERS();   \
    ADD_EVENT(redraw)

        REGISTER_CLASS_DECL(xaml, canvas, "xaml/ui/controls")
        {
            ADD_CTOR_DEF();
            ADD_CANVAS_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_CANVAS_HPP
