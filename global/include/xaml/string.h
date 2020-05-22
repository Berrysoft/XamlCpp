#ifndef XAML_STRING_H
#define XAML_STRING_H

#ifdef __cplusplus
#include <ostream>
#include <string>
#include <string_view>
#include <xaml/ptr.hpp>
#else
#include <stdbool.h>
#endif // __cplusplus

#include <xaml/object.h>

#ifndef U
#ifdef UNICODE
#ifdef __cplusplus
#define U(x) reinterpret_cast<char const*>(u8##x)
#else
#define U(x) u8##x
#endif // __cplusplus
#else
#define U(x) x
#endif // UNICODE
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
EXTERN_C XAML_API xaml_result XAML_CALL xaml_string_new_view(char const*, xaml_string**) XAML_NOEXCEPT;

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
    char const* data;
    XAML_THROW_IF_FAILED(str->get_data(&data));
    std::int32_t length;
    XAML_THROW_IF_FAILED(str->get_length(&length));
    return std::string_view(data, length);
}

inline xaml_result to_string_view(xaml_ptr<xaml_string> const& str, std::string_view* view) noexcept
{
    char const* data;
    XAML_RETURN_IF_FAILED(str->get_data(&data));
    std::int32_t length;
    XAML_RETURN_IF_FAILED(str->get_length(&length));
    *view = std::string_view(data, length);
    return XAML_S_OK;
}

inline std::string to_string(xaml_ptr<xaml_string> const& str)
{
    return (std::string)to_string_view(str);
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

#ifdef UNICODE
XAML_API std::wstring to_wstring(xaml_ptr<xaml_string> const&);
XAML_API std::wstring to_wstring(std::string_view);
XAML_API xaml_result to_wstring(xaml_ptr<xaml_string> const&, std::wstring*) noexcept;
#endif // UNICODE

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
