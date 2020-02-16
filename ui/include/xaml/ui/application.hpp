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
    class application : public meta_class_impl<application>
    {
    private:
        friend class window;

        int wnd_num{ 0 };
        std::vector<string_t> m_cmd_lines{};
        std::unordered_map<std::string_view, std::shared_ptr<module>> m_modules{};

    private:
        XAML_UI_API application(int argc, char_t** argv);

    public:
        XAML_UI_API virtual ~application();

        array_view<string_t> get_cmd_lines() const noexcept { return m_cmd_lines; }

        XAML_UI_API void add_module(std::string_view path);

        XAML_UI_API int run();

        XAML_UI_API static std::shared_ptr<application> init(int argc, char_t** argv);
        static std::shared_ptr<application> init() { return init(0, nullptr); }
#if defined(WIN32) || defined(__MINGW32__)
        XAML_UI_API static std::shared_ptr<application> init(LPTSTR lpCmdLine);
#endif // WIN32 || __MINGW32__
        XAML_UI_API static std::shared_ptr<application> current();

#ifdef XAML_UI_WINDOWS
        XAML_UI_API HFONT __default_font() const;
#endif // XAML_UI_WINDOWS

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, application);
        }
    };
} // namespace xaml

#endif // !XAML_UI_APPLICATION_HPP
