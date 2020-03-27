#ifndef XAML_UI_WINDOW_HPP
#define XAML_UI_WINDOW_HPP

#include <atomic>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/container.hpp>
#include <xaml/ui/menu_bar.hpp>

namespace xaml
{
    struct native_window;
    class window;

    template <>
    struct type_guid<window>
    {
        static constexpr guid value{ 0xf65be476, 0xbef3, 0x4d90, 0xa6, 0xcd, 0x89, 0x33, 0x38, 0x9c, 0x2c, 0x53 };
    };

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
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    private:
        static void on_destroy(void* w, void* arg);
        static int on_configure_event(void* widget, void* event, void* data);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    public:
        void __on_did_resize();
        void __on_did_move();
        bool __on_should_close();
#endif // XAML_UI_COCOA

    public:
        XAML_UI_API void __draw(rectangle const& region) override;

    protected:
        XAML_UI_API void __parent_redraw() override;

    protected:
#ifdef XAML_UI_COCOA
        XAML_UI_API void draw_visible() override;
#endif // XAML_UI_COCOA

        XAML_UI_API void draw_size() override;
        XAML_UI_API virtual void draw_title();
        XAML_UI_API virtual void draw_child();
        XAML_UI_API virtual void draw_resizable();
        XAML_UI_API virtual void draw_menu_bar();

    public:
        XAML_UI_API window();
        XAML_UI_API virtual ~window() override;

    public:
        std::shared_ptr<control> get_root_window() override { return std::static_pointer_cast<control>(shared_from_this()); }

        XAML_UI_API void show();
        XAML_UI_API void close();
        XAML_UI_API void hide();

        PROP_EVENT(resizable, bool)
        EVENT(resizable_changed, std::shared_ptr<window>, bool)

    private:
        std::atomic_bool m_resizing{ false };

        EVENT(location_changed, std::shared_ptr<window>, point)

        PROP_CONSTEXPR_EVENT(location, point)

    public:
        double get_x() const { return get_location().x; }
        double get_y() const { return get_location().y; }
        void set_x(double value)
        {
            if (m_location.x != value)
            {
                m_location.x = value;
                m_location_changed(std::static_pointer_cast<window>(shared_from_this()), get_location());
            }
        }
        void set_y(double value)
        {
            if (m_location.y != value)
            {
                m_location.y = value;
                m_location_changed(std::static_pointer_cast<window>(shared_from_this()), get_location());
            }
        }

    public:
        XAML_UI_API rectangle get_client_region() const;

#ifdef XAML_UI_WINDOWS
    public:
        XAML_UI_API point __get_real_location() const;
        XAML_UI_API void __set_real_location(point value);

        XAML_UI_API rectangle __get_real_client_region() const;
#endif // XAML_UI_WINDOWS

        EVENT(closing, std::shared_ptr<window>, std::shared_ptr<meta_box<bool>>)

        EVENT(title_changed, std::shared_ptr<window>, string_view_t)
        PROP_STRING_EVENT(title)

    private:
        std::shared_ptr<menu_bar> m_menu_bar{ nullptr };

    public:
        std::shared_ptr<menu_bar> get_menu_bar() const noexcept { return m_menu_bar; }
        void set_menu_bar(std::shared_ptr<menu_bar> const& value) { m_menu_bar = value; }

    public:
        XAML_UI_API double get_dpi() const;

    public:
#define ADD_WINDOW_MEMBERS()   \
    ADD_CONTAINER_MEMBERS();   \
    ADD_PROP(title);           \
    ADD_EVENT(title_changed);  \
    ADD_PROP_EVENT(location);  \
    ADD_PROP(x);               \
    ADD_PROP(y);               \
    ADD_EVENT(closing);        \
    ADD_PROP_EVENT(resizable); \
    ADD_PROP(menu_bar)

        REGISTER_CLASS_DECL(xaml, window, "xaml/ui")
        {
            ADD_WINDOW_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_WINDOW_HPP
