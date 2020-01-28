#ifndef XAML_MODULE_MODULE_HPP
#define XAML_MODULE_MODULE_HPP

#include <string_view>
#include <xaml/utility.hpp>

#if defined(WIN32) || defined(__MINGW32__)
#include <Windows.h>
#endif // WIN32 || __MINGW32__

namespace xaml
{
#if defined(WIN32) || defined(__MINGW32__)
    inline constexpr std::wstring_view module_extension{ L".dll" };
#elif defined(__APPLE__)
    inline constexpr std::string_view module_extension{ ".dylib" };
#else
    inline constexpr std::string_view module_extension{ ".so" };
#endif

    class module
    {
    public:
#if defined(WIN32) || defined(__MINGW32__)
        using native_handle_type = HMODULE;
#else
        using native_handle_type = void*;
#endif
    private:
        native_handle_type m_handle{ nullptr };

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

        module(module&& m) : m_handle(m.m_handle)
        {
            m.m_handle = nullptr;
        }

        module& operator=(module&& m)
        {
            std::swap(m_handle, m.m_handle);
            return *this;
        }

        virtual ~module() { close(); }

        XAML_META_API void open(std::string_view name);
        XAML_META_API void close();

    protected:
        XAML_META_API void* get_method(std::string_view name);

    public:
        XAML_META_API void register_meta() noexcept;
    };
} // namespace xaml

#endif // !XAML_MODULE_MODULE_HPP
