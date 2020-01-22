#ifndef XAML_UI_APPLICATION_HPP
#define XAML_UI_APPLICATION_HPP

#include <memory>
#include <string>
#include <tuple>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/meta/module.hpp>
#include <xaml/strings.hpp>
#include <xaml/ui/objc.hpp>

#if defined(XAML_UI_WINDOWS)
#include <Windows.h>
#elif defined(XAML_UI_WINRT)
#include "winrt/Windows.UI.Xaml.Controls.h"
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
#endif
        XAML_API static std::shared_ptr<application> current();

    private:
        std::unordered_map<std::type_index, std::tuple<void*, std::function<void(void*)>>> m_token_map;
        std::unordered_map<string_view_t, module> m_module_map;

    public:
        template <typename T>
        void add_module() noexcept
        {
            auto t = std::type_index(typeid(T));
            auto it = m_token_map.find(t);
            if (it == m_token_map.end())
            {
                m_token_map.emplace(t, std::make_tuple<void*, std::function<void(void*)>>(T::init_components(), &T::cleanup_components));
            }
        }

        XAML_API void add_module(string_view_t path);

        void cleanup_modules()
        {
            for (auto& p : m_token_map)
            {
                std::get<1>(p.second)(std::get<0>(p.second));
            }
        }

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
