#ifndef XAML_UI_APPLICATION_HPP
#define XAML_UI_APPLICATION_HPP

#include <memory>
#include <string>
#include <vector>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/strings.hpp>
#include <xaml/ui/objc.hpp>

#if defined(XAML_UI_WINDOWS) || defined(XAML_UI_WINRT)
#include <Windows.h>
#endif // XAML_UI_WINDOWS

namespace xaml
{
    class application : public meta_class_impl<application>
    {
    private:
        friend class window;

        int wnd_num{ 0 };
        std::vector<string_t> m_cmd_lines{};

#ifdef XAML_UI_WINDOWS
        ULONG_PTR m_gdiplus_oken;
#endif // XAML_UI_WINDOWS

    private:
        application(int argc, char_t** argv)
        {
            m_cmd_lines.assign(argv, argv + argc);
        }
#if defined(XAML_UI_WINDOWS) && defined(UNICODE)
        XAML_API application(LPWSTR lpCmdLine);
#endif

        XAML_API void init_components();

    public:
        XAML_API virtual ~application();

        const std::vector<string_t>& get_cmd_lines() const noexcept { return m_cmd_lines; }
        XAML_API int run();

        XAML_API static std::shared_ptr<application> init(int argc, char_t** argv);
        static std::shared_ptr<application> init() { return init(0, nullptr); }
#if defined(XAML_UI_WINDOWS) && defined(UNICODE)
        XAML_API static std::shared_ptr<application> init(LPWSTR lpCmdLine);
#endif // XAML_UI_WINDOWS
        XAML_API static std::shared_ptr<application> current();

#ifdef XAML_UI_WINDOWS
        XAML_API HFONT __default_font() const;
#endif // XAML_UI_WINDOWS

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, application);
        }
    };
} // namespace xaml

#endif // !XAML_UI_APPLICATION_HPP
