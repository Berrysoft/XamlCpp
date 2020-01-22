#ifndef XAML_MODULE_MODULE_HPP
#define XAML_MODULE_MODULE_HPP

#include <xaml/strings.hpp>
#include <xaml/utility.hpp>

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

namespace xaml
{
    class module
    {
    public:
#ifdef WIN32
        using native_handle_type = HMODULE;
#else
        using native_handle_type = void*;
#endif // WIN32
    private:
        native_handle_type m_handle{ nullptr };

    public:
        constexpr native_handle_type get_handle() const noexcept { return m_handle; }

        constexpr operator bool() const noexcept { return m_handle; }

    protected:
        void set_handle(native_handle_type value) noexcept { m_handle = value; }

    public:
        module() {}
        module(string_view_t path) : module() { open(path); }

        ~module() { close(); }

        XAML_API void open(string_view_t path);
        XAML_API void* get_method(std::string_view name);
        XAML_API void close();

        XAML_API void register_meta() noexcept;

    private:
        void* m_token{ nullptr };

    public:
        XAML_API void init_components() noexcept;
        XAML_API void cleanup_components() noexcept;
    };
} // namespace xaml

#endif // !XAML_MODULE_MODULE_HPP
