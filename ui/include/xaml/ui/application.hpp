#ifndef XAML_UI_APPLICATION_HPP
#define XAML_UI_APPLICATION_HPP

#include <memory>
#include <string>
#include <tuple>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <xaml/array_view.hpp>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/meta/module.hpp>
#include <xaml/strings.hpp>
#include <xaml/ui/objc.hpp>

#if defined(WIN32) || defined(__MINGW32__)
#include <Windows.h>
#endif // WIN32 || __MINGW32__

namespace xaml
{
    class window;

    class application : public meta_class
    {
    private:
        friend class window;

        std::vector<string_t> m_cmd_lines{};

        std::shared_ptr<xaml::window> m_main_wnd{ nullptr };
        int m_quit_value{ 0 };

    private:
        XAML_UI_API application(int argc, char_t const* const* argv);

    public:
        XAML_UI_API virtual ~application();

        array_view<string_t> get_cmd_lines() const noexcept { return m_cmd_lines; }

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

        XAML_UI_API static std::shared_ptr<application> init(int argc, char_t const* const* argv);
        static std::shared_ptr<application> init() { return init(0, nullptr); }
#if defined(WIN32) || defined(__MINGW32__)
        XAML_UI_API static std::shared_ptr<application> init(LPTSTR lpCmdLine);
#endif // WIN32 || __MINGW32__
        XAML_UI_API static std::shared_ptr<application> current();

#ifdef XAML_UI_WINDOWS
        XAML_UI_API HFONT __default_font() const;
        XAML_UI_API HFONT __default_font(HWND hWnd) const;
#endif // XAML_UI_WINDOWS

        REGISTER_CLASS_DECL()
        {
            REGISTER_TYPE(xaml, application);
        }
    };
} // namespace xaml

#endif // !XAML_UI_APPLICATION_HPP
