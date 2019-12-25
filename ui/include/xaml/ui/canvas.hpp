#ifndef XAML_UI_CANVAS_HPP
#define XAML_UI_CANVAS_HPP

#include <xaml/ui/control.hpp>
#include <xaml/ui/drawing.hpp>

#ifdef XAML_UI_WINDOWS
#include <wil/resource.h>
#endif // XAML_UI_WINDOWS

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

        drawing_context(native_handle_type handle) : m_handle(handle) {}

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
