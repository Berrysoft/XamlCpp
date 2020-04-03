#ifndef XAML_UI_APPLICATION_HPP
#define XAML_UI_APPLICATION_HPP

#include <memory>
#include <string>
#include <vector>
#include <xaml/array_view.hpp>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/meta/module.hpp>
#include <xaml/strings.hpp>
#include <xaml/ui/objc.hpp>

namespace xaml
{
    class window;

    enum class application_theme
    {
        light,
        dark
    };

    class application
    {
    private:
        friend class window;

    protected:
        XAML_UI_API application(int argc, uchar_t const* const* argv);

    public:
        XAML_UI_API virtual ~application();

    private:
        std::vector<ustring_t> m_cmd_lines{};

    public:
        array_view<ustring_t> get_cmd_lines() const noexcept { return m_cmd_lines; }

    private:
        std::shared_ptr<xaml::window> m_main_wnd{ nullptr };
        int m_quit_value{ 0 };

    public:
        std::shared_ptr<window> get_main_window() const { return m_main_wnd; }

    protected:
        void set_main_window(std::shared_ptr<window> value) { m_main_wnd = value; }

    public:
        void window_added(std::shared_ptr<window> value)
        {
            if (!m_main_wnd) set_main_window(value);
        }
        void window_removed(std::shared_ptr<window> value)
        {
            if (m_main_wnd == value)
            {
                m_main_wnd = nullptr;
                quit();
            }
        }

    public:
        XAML_UI_API int run();
        XAML_UI_API void quit(int value = 0);

        XAML_UI_API static std::shared_ptr<application> init();
        XAML_UI_API static std::shared_ptr<application> init(int argc, uchar_t const* const* argv);

        XAML_UI_API static std::shared_ptr<application> current();

        XAML_UI_API application_theme get_theme() const;

#ifdef XAML_UI_WINDOWS
        XAML_UI_API void* __default_font(unsigned int udpi) const;
#endif // XAML_UI_WINDOWS
    };
} // namespace xaml

#endif // !XAML_UI_APPLICATION_HPP
