#ifndef XAML_UI_WINDOW_HPP
#define XAML_UI_WINDOW_HPP

#include <atomic>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/container.hpp>

namespace xaml
{
    struct native_window;

    class window : public container
    {
    public:
        using native_window_type = std::shared_ptr<native_window>;

    private:
        native_window_type m_window{ nullptr };

    public:
        native_window_type get_window() const noexcept { return m_window; }

    protected:
        void set_window(native_window_type value) { m_window = value; }

#ifdef XAML_UI_WINDOWS
    public:
        XAML_UI_API virtual std::optional<std::intptr_t> __wnd_proc(window_message const& msg) override;
        XAML_UI_API void __copy_hdc(rectangle const& region, HDC hDC);
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    private:
        static void on_destroy(void* w, void* arg);
        static int on_configure_event(void* widget, void* event, void* data);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    public:
        void __on_did_resize();
        bool __on_should_close();
#endif // XAML_UI_COCOA

    public:
        XAML_UI_API void __draw(rectangle const& region) override;

    protected:
        XAML_UI_API void __parent_redraw() override;

    protected:
        XAML_UI_API virtual void draw_size() override;
        XAML_UI_API virtual void draw_title();
        XAML_UI_API virtual void draw_child();
        XAML_UI_API virtual void draw_resizable();

    public:
        XAML_UI_API window();
        XAML_UI_API virtual ~window() override;

    public:
        std::shared_ptr<control> get_root_window() override { return std::static_pointer_cast<control>(shared_from_this()); }

        XAML_UI_API void show();
        XAML_UI_API void close();
        XAML_UI_API void hide();

        PROP_EVENT(resizable, bool)
        EVENT(resizable_changed, window&, bool)

    private:
        std::atomic<bool> m_resizing{ false };

        EVENT(location_changed, window&, point)

        PROP_CONSTEXPR_EVENT(location, point)

    public:
        double get_x() const { return get_location().x; }
        double get_y() const { return get_location().y; }
        void set_x(double value)
        {
            if (m_location.x != value)
            {
                m_location.x = value;
                m_location_changed(*this, get_location());
            }
        }
        void set_y(double value)
        {
            if (m_location.y != value)
            {
                m_location.y = value;
                m_location_changed(*this, get_location());
            }
        }

    public:
        XAML_UI_API point __get_real_location() const;
        XAML_UI_API void __set_real_location(point value);

        XAML_UI_API rectangle get_client_region() const;
        XAML_UI_API rectangle __get_real_client_region() const;

        EVENT(closing, window&, bool&)

        EVENT(title_changed, window&, string_view_t)
        PROP_STRING_EVENT(title)

    public:
        double get_dpi() const;

    public:
#define ADD_WINDOW_MEMBERS()  \
    ADD_CONTAINER_MEMBERS();  \
    ADD_PROP(title);          \
    ADD_EVENT(title_changed); \
    ADD_PROP_EVENT(location); \
    ADD_PROP(x);              \
    ADD_PROP(y);              \
    ADD_EVENT(closing);       \
    ADD_PROP_EVENT(resizable)

        REGISTER_CLASS_DECL(xaml, window)
        {
            ADD_WINDOW_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_WINDOW_HPP
