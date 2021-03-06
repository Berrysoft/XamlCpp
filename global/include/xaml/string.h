#ifndef XAML_STRING_H
#define XAML_STRING_H

#ifdef __cplusplus
    #include <ostream>
    #include <string>
    #include <string_view>
    #include <xaml/ptr.hpp>

    #ifdef XAML_WIN32
        #include <memory_resource>
    #endif // XAML_WIN32
#endif // __cplusplus

#include <xaml/object.h>

#ifndef U
    #ifdef __cpp_char8_t
        #define U(x) reinterpret_cast<char const*>(u8##x)
    #else
        #define U(x) u8##x
    #endif // __cpp_char8_t
#endif // !U

#ifndef U_
    #define U_(x) U(x)
#endif // !U_

XAML_CLASS(xaml_string, { 0xc8386ec4, 0xd28d, 0x422f, { 0x9e, 0x44, 0x36, 0xaa, 0x77, 0x63, 0x39, 0xd3 } })

#define XAML_STRING_VTBL(type)                        \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));        \
    XAML_METHOD(get_length, type, XAML_STD int32_t*); \
    XAML_METHOD(get_data, type, char const**)

XAML_DECL_INTERFACE_(xaml_string, xaml_object)
{
    XAML_DECL_VTBL(xaml_string, XAML_STRING_VTBL);
};

EXTERN_C XAML_API xaml_result XAML_CALL xaml_string_new(char const*, xaml_string**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_string_new_length(char const*, XAML_STD int32_t, xaml_string**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_string_new_view(char const*, xaml_string**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_string_new_view_length(char const*, XAML_STD int32_t, xaml_string**) XAML_NOEXCEPT;

EXTERN_C XAML_API xaml_result XAML_CALL xaml_string_empty(xaml_string**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_string_equals(xaml_string*, xaml_string*, bool*) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_string_clone(xaml_string*, xaml_string**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_string_concat(xaml_string*, xaml_string*, xaml_string**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_string_substr(xaml_string*, XAML_STD int32_t, XAML_STD int32_t, xaml_string**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_API xaml_result XAML_CALL xaml_string_new(std::string&&, xaml_string**) noexcept;
XAML_API xaml_result XAML_CALL xaml_string_new(std::string_view, xaml_string**) noexcept;

XAML_API xaml_result XAML_CALL xaml_string_new_view(std::string_view, xaml_string**) noexcept;

XAML_API std::ostream& operator<<(std::ostream&, xaml_ptr<xaml_string> const&);

inline std::string_view to_string_view(xaml_ptr<xaml_string> const& str)
{
    if (str)
    {
        char const* data;
        XAML_THROW_IF_FAILED(str->get_data(&data));
        std::int32_t length;
        XAML_THROW_IF_FAILED(str->get_length(&length));
        return std::string_view(data, length);
    }
    else
    {
        return {};
    }
}

inline xaml_result to_string_view(xaml_ptr<xaml_string> const& str, std::string_view* view) noexcept
{
    if (str)
    {
        char const* data;
        XAML_RETURN_IF_FAILED(str->get_data(&data));
        std::int32_t length;
        XAML_RETURN_IF_FAILED(str->get_length(&length));
        *view = std::string_view(data, length);
    }
    else
    {
        *view = {};
    }
    return XAML_S_OK;
}

inline std::string to_string(xaml_ptr<xaml_string> const& str)
{
    return static_cast<std::string>(to_string_view(str));
}

inline xaml_result to_string(xaml_ptr<xaml_string> const& str, std::string* s) noexcept
try
{
    std::string_view view;
    XAML_RETURN_IF_FAILED(to_string_view(str, &view));
    *s = view;
    return XAML_S_OK;
}
XAML_CATCH_RETURN()

    #ifdef XAML_WIN32
XAML_API std::wstring to_wstring(std::string_view);

inline xaml_result to_wstring(std::string_view str, std::wstring* pres) noexcept
try
{
    *pres = to_wstring(str);
    return XAML_S_OK;
}
XAML_CATCH_RETURN()

inline std::wstring to_wstring(xaml_ptr<xaml_string> const& str)
{
    return to_wstring(to_string_view(str));
}

inline xaml_result to_wstring(xaml_ptr<xaml_string> const& str, std::wstring* presult) noexcept
{
    std::string_view view;
    XAML_RETURN_IF_FAILED(to_string_view(str, &view));
    return to_wstring(view, presult);
}

XAML_API std::pmr::wstring to_pmr_wstring(std::string_view, std::pmr::polymorphic_allocator<wchar_t> = {});

XAML_API std::string to_string(std::wstring_view);

inline xaml_result to_string(std::wstring_view wstr, std::string* pres) noexcept
try
{
    *pres = to_string(wstr);
    return XAML_S_OK;
}
XAML_CATCH_RETURN()

XAML_API std::pmr::string to_pmr_string(std::wstring_view, std::pmr::polymorphic_allocator<char> = {});

inline xaml_result XAML_CALL xaml_string_new(std::wstring_view wstr, xaml_string** ptr) noexcept
{
    std::string str;
    XAML_RETURN_IF_FAILED(to_string(wstr, &str));
    return xaml_string_new(std::move(str), ptr);
}

struct __xaml_codecvt_pool_impl
{
private:
    std::pmr::polymorphic_allocator<wchar_t> m_wide_allocator;
    std::pmr::polymorphic_allocator<char> m_allocator;

public:
    __xaml_codecvt_pool_impl(std::pmr::memory_resource* res) noexcept : m_wide_allocator{ res }, m_allocator{ res } {}

    XAML_API std::wstring_view operator()(std::string_view);

    inline xaml_result operator()(std::string_view str, std::wstring_view* pres) noexcept
    try
    {
        *pres = operator()(str);
        return XAML_S_OK;
    }
    XAML_CATCH_RETURN()

    std::wstring_view operator()(xaml_ptr<xaml_string> const& str)
    {
        return operator()(to_string_view(str));
    }

    xaml_result operator()(xaml_ptr<xaml_string> const& str, std::wstring_view* presult) noexcept
    {
        std::string_view view;
        XAML_RETURN_IF_FAILED(to_string_view(str, &view));
        return operator()(view, presult);
    }

    XAML_API std::string_view operator()(std::wstring_view);

    inline xaml_result operator()(std::wstring_view wstr, std::string_view* pres) noexcept
    try
    {
        *pres = operator()(wstr);
        return XAML_S_OK;
    }
    XAML_CATCH_RETURN()

    xaml_result operator()(std::wstring_view wstr, xaml_string** ptr) noexcept
    {
        std::string_view view;
        XAML_RETURN_IF_FAILED(operator()(wstr, &view));
        return xaml_string_new_view(view, ptr);
    }
};

template <typename Res = std::pmr::monotonic_buffer_resource>
struct xaml_codecvt_pool : private Res, public __xaml_codecvt_pool_impl
{
    xaml_codecvt_pool() noexcept(std::is_nothrow_constructible_v<Res>) : Res(), __xaml_codecvt_pool_impl(this) {}
    template <typename... Args>
    xaml_codecvt_pool(Args&&... args) noexcept(noexcept(Res{ std::forward<Args>(args)... })) : Res(std::forward<Args>(args)...), __xaml_codecvt_pool_impl(this)
    {
    }

    constexpr std::pmr::memory_resource* resource() noexcept { return this; }
};
    #endif // XAML_WIN32

namespace std
{
    template <>
    struct hash<xaml_ptr<xaml_string>>
    {
        size_t operator()(xaml_ptr<xaml_string> const& str) const noexcept
        {
            std::string_view view{};
            XAML_ASSERT_SUCCEEDED(to_string_view(str, &view));
            return hash<std::string_view>{}(view);
        }
    };
} // namespace std

inline std::size_t hash_value(xaml_ptr<xaml_string> const& str) noexcept
{
    return std::hash<xaml_ptr<xaml_string>>{}(str);
}
#endif // __cplusplus

#endif // !XAML_STRING_H
