#ifndef XAML_UI_WINDOW_HPP
#define XAML_UI_WINDOW_HPP

#include <atomic>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/container.hpp>

namespace xaml
{
#ifdef XAML_UI_WINDOWS
    LRESULT CALLBACK wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
#endif // XAML_UI_WINDOWS

    class window : public container, public meta_class_impl<window>
    {
    private:
        static constexpr std::string_view namespace_name = "xaml";
        static constexpr std::string_view class_name = "window";

#ifdef XAML_UI_WINDOWS
    public:
        virtual std::optional<LRESULT> wnd_proc(window_message const& msg) override;
#endif

    public:
        void draw(rectangle const& region) override;

    public:
        window();
        virtual ~window() override;

        void show();

    private:
        std::atomic<bool> resizing{ false };

        point m_location{ 0, 0 };

        EVENT(location_changed, window const&, point)

    public:
        constexpr point get_location() const noexcept { return m_location; }
        void set_location(point value)
        {
            if (m_location != value)
            {
                m_location = value;
                m_location_changed(*this, get_location());
            }
        }

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

        EVENT(closing, window const&, bool&)

    private:
        string_t m_title{};

    public:
        string_view_t get_title() const noexcept { return m_title; }
        void set_title(string_view_t value) { m_title = (string_t)value; }

#define ADD_WINDOW_MEMBERS()     \
    ADD_CONTAINER_MEMBERS();     \
    ADD_PROP(title);             \
    ADD_PROP(location);          \
    ADD_PROP(x);                 \
    ADD_PROP(y);                 \
    ADD_EVENT(location_changed); \
    ADD_EVENT(closing)

        static void register_class() noexcept
        {
            REGISTER_TYPE();
            ADD_CTOR_DEF();
            ADD_WINDOW_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_WINDOW_HPP
