#ifndef XAML_UI_CANVAS_HPP
#define XAML_UI_CANVAS_HPP

#include <xaml/ui/control.hpp>
#include <xaml/ui/drawing.hpp>

namespace xaml
{
    class drawing_context
    {
    public:
#ifdef XAML_UI_WINDOWS
        using native_handle_type = HDC;
#elif defined(XAML_UI_GTK3)
        using native_handle_type = cairo_t*;
#endif // XAML_UI_WINDOWS

    private:
        native_handle_type m_handle;

    public:
        constexpr native_handle_type get_handle() const noexcept { return m_handle; }

    public:
        void draw_arc(rectangle const& region, double start_angle, double end_angle);
        void draw_ellipse(rectangle const& region);
        void draw_line(point startp, point endp);
        void draw_rect(rectangle const& rect);
        void draw_round_rect(rectangle const& rect, size round);
        void draw_string(point p, string_view_t str);
    };

    class canvas : public common_control, public meta_class_impl<canvas>
    {
    public:
        canvas();
        ~canvas() override;

#ifdef XAML_UI_WINDOWS
    public:
        virtual std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

    public:
        EVENT(redraw, canvas const&, drawing_context&)

        void __draw(rectangle const& region) override;
    };
} // namespace xaml

#endif // !XAML_UI_CANVAS_HPP
