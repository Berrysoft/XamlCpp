#ifndef XAML_UI_WINDOW_HPP
#define XAML_UI_WINDOW_HPP

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
        virtual LRESULT wnd_proc(window_message const& msg) override;

        void draw(rectangle const& region) override;
#endif

    public:
        window();
        virtual ~window() override;

        void show();

    private:
        point m_location{ 0, 0 };

    public:
        constexpr point get_location() const noexcept { return m_location; }
        void set_location(point value) { m_location = value; }

        double get_x() const { return get_location().x; }
        double get_y() const { return get_location().y; }
        void set_x(double value) { set_location({ value, get_y() }); }
        void set_y(double value) { set_location({ get_x(), value }); }

    private:
        string_t m_title{};

    public:
        string_view_t get_title() const noexcept { return m_title; }
        void set_title(string_view_t value) { m_title = (string_t)value; }

        static void register_class() noexcept
        {
            REGISTER_TYPE();
            ADD_CTOR_DEF();
            ADD_PROP(title);
        }
    };
} // namespace xaml

#endif // !XAML_UI_WINDOW_HPP
