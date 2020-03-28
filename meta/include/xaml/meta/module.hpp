#ifndef XAML_MODULE_MODULE_HPP
#define XAML_MODULE_MODULE_HPP

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>
#include <xaml/utility.hpp>

namespace xaml
{
#if defined(WIN32) || defined(__MINGW32__)
    inline std::filesystem::path module_extension{ ".dll" };
#elif defined(__APPLE__)
    inline std::filesystem::path module_extension{ ".dylib" };
#else
    inline std::filesystem::path module_extension{ ".so" };
#endif // WIN32 || __MINGW32__

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
        module(std::filesystem::path const& name) : module() { open(name); }

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
        std::vector<std::filesystem::path> m_search_dir;

    public:
        void add_search_dir(std::filesystem::path const& dir) { m_search_dir.emplace_back(dir); }

        XAML_META_API void open(std::filesystem::path const& name);
        XAML_META_API void close();

        XAML_META_API void* get_method(std::string_view name) const;
    };
} // namespace xaml

#endif // !XAML_MODULE_MODULE_HPP
