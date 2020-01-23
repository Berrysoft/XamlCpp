#ifndef XAML_MODULE_MODULE_HPP
#define XAML_MODULE_MODULE_HPP

#include <string_view>
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
        void* m_token{ nullptr };

    public:
        constexpr native_handle_type get_handle() const noexcept { return m_handle; }

        constexpr operator bool() const noexcept { return m_handle; }

    protected:
        void set_handle(native_handle_type value) noexcept { m_handle = value; }

    public:
        module() {}
        module(std::string_view name) : module() { open(name); }

        module(module const&) = delete;
        module& operator=(module const&) = delete;

        module(module&& m) : m_handle(m.m_handle), m_token(m.m_token)
        {
            m.m_handle = nullptr;
            m.m_token = nullptr;
        }

        module& operator=(module&& m)
        {
            std::swap(m_handle, m.m_handle);
            std::swap(m_token, m.m_token);
            return *this;
        }

        ~module() { close(); }

        XAML_API void open(std::string_view name);
        XAML_API void* get_method(std::string_view name);
        XAML_API void close();

        XAML_API void register_meta() noexcept;

        XAML_API void init_components() noexcept;
        XAML_API void cleanup_components() noexcept;
    };
} // namespace xaml

#endif // !XAML_MODULE_MODULE_HPP
