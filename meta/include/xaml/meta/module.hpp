#ifndef XAML_MODULE_MODULE_HPP
#define XAML_MODULE_MODULE_HPP

#include <string>
#include <string_view>
#include <vector>
#include <xaml/utility.hpp>

#ifndef P
#if defined(WIN32) || defined(__MINGW32__)
#define P(x) L##x
#else
#define P(x) x
#endif // WIN32 || __MINGW32__
#endif // !P

namespace xaml
{
#if defined(WIN32) || defined(__MINGW32__)
    using path_char_t = wchar_t;
    using path_string_t = std::wstring;
    using path_string_view_t = std::wstring_view;
#else
    using path_char_t = char;
    using path_string_t = std::string;
    using path_string_view_t = std::string_view;
#endif // WIN32 || __MINGW32__

#if defined(WIN32) || defined(__MINGW32__)
    inline constexpr path_string_view_t module_extension{ P(".dll") };
#elif defined(__APPLE__)
    inline constexpr path_string_view_t module_extension{ P(".dylib") };
#else
    inline constexpr path_string_view_t module_extension{ P(".so") };
#endif // WIN32 || __MINGW32__

    XAML_META_API std::vector<path_string_t> get_module_search_path();

    class module
    {
    public:
        using native_handle_type = void*;

    private:
        native_handle_type m_handle{ nullptr };

    public:
        constexpr native_handle_type get_handle() const noexcept { return m_handle; }

        constexpr operator bool() const noexcept { return m_handle; }

    protected:
        void set_handle(native_handle_type value) noexcept { m_handle = value; }

    public:
        module() {}
        module(path_string_view_t name) : module() { open(name); }

        module(module const&) = delete;
        module& operator=(module const&) = delete;

        module(module&& m) noexcept : m_handle(m.m_handle)
        {
            m.m_handle = nullptr;
        }

        module& operator=(module&& m) noexcept
        {
            std::swap(m_handle, m.m_handle);
            return *this;
        }

        virtual ~module() { close(); }

    private:
        std::vector<path_string_t> m_search_dir;

    public:
        void add_search_dir(path_string_view_t dir) { m_search_dir.emplace_back(dir); }

        XAML_META_API void open(path_string_view_t name);
        XAML_META_API void close();

        XAML_META_API void* get_method(std::string_view name) const;
    };
} // namespace xaml

#endif // !XAML_MODULE_MODULE_HPP
