#ifndef XAML_STRING_H
#define XAML_STRING_H

#ifdef __cplusplus
#include <string>
#include <string_view>
#include <xaml/ptr.hpp>
#else
#include <stdbool.h>
#endif // __cplusplus

#include <xaml/object.h>

#ifdef UNICODE
typedef wchar_t xaml_char_t;
#else
typedef char xaml_char_t;
#endif // UNICODE

#ifndef U
#ifdef UNICODE
#define U(x) L##x
#else
#define U(x) x
#endif // UNICODE
#endif // !U

XAML_CLASS(xaml_string, { 0xc8386ec4, 0xd28d, 0x422f, { 0x9e, 0x44, 0x36, 0xaa, 0x77, 0x63, 0x39, 0xd3 } })

#define XAML_STRING_VTBL(type)                         \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));         \
    XAML_METHOD(get_length, type, XAML_CSTD int32_t*); \
    XAML_METHOD(get_data, type, xaml_char_t const**);  \
    XAML_METHOD(equals, type, xaml_string*, bool*)

XAML_DECL_INTERFACE_(xaml_string, xaml_object)
{
    XAML_DECL_VTBL(xaml_string, XAML_STRING_VTBL);
};

EXTERN_C XAML_API xaml_result XAML_CALL xaml_string_new(xaml_char_t const*, xaml_string**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_string_new_view(xaml_char_t const*, xaml_string**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_string_new_utf8(char const*, xaml_string**) XAML_NOEXCEPT;

#ifdef __cplusplus
using xaml_std_string_t = std::basic_string<xaml_char_t>;
using xaml_std_string_view_t = std::basic_string_view<xaml_char_t>;

XAML_API xaml_result XAML_CALL xaml_string_new(xaml_std_string_t&&, xaml_string**) noexcept;
XAML_API xaml_result XAML_CALL xaml_string_new(xaml_std_string_view_t, xaml_string**) noexcept;

XAML_API xaml_result XAML_CALL xaml_string_new_view(xaml_std_string_view_t, xaml_string**) noexcept;

XAML_API xaml_result XAML_CALL xaml_string_new_utf8(std::string&&, xaml_string**) noexcept;
XAML_API xaml_result XAML_CALL xaml_string_new_utf8(std::string_view, xaml_string**) noexcept;

XAML_API std::string to_string_utf8(xaml_ptr<xaml_string> const&) noexcept;

inline xaml_ptr<xaml_string> to_xaml_string(xaml_char_t const* str)
{
    xaml_ptr<xaml_string> res;
    XAML_THROW_IF_FAILED(xaml_string_new(str, &res));
    return res;
}

inline xaml_ptr<xaml_string> to_xaml_string(xaml_std_string_t&& str)
{
    xaml_ptr<xaml_string> res;
    XAML_THROW_IF_FAILED(xaml_string_new(std::move(str), &res));
    return res;
}

inline xaml_ptr<xaml_string> to_xaml_string(xaml_std_string_view_t str)
{
    xaml_ptr<xaml_string> res;
    XAML_THROW_IF_FAILED(xaml_string_new(str, &res));
    return res;
}

inline xaml_std_string_view_t to_string_view_t(xaml_ptr<xaml_string> const& str)
{
    xaml_char_t const* data;
    XAML_THROW_IF_FAILED(str->get_data(&data));
    std::int32_t length;
    XAML_THROW_IF_FAILED(str->get_length(&length));
    return xaml_std_string_view_t(data, length);
}

inline xaml_result to_string_view_t(xaml_ptr<xaml_string> const& str, xaml_std_string_view_t& view) noexcept
{
    xaml_char_t const* data;
    XAML_RETURN_IF_FAILED(str->get_data(&data));
    std::int32_t length;
    XAML_RETURN_IF_FAILED(str->get_length(&length));
    view = xaml_std_string_view_t(data, length);
    return XAML_S_OK;
}

inline xaml_std_string_t to_string_t(xaml_ptr<xaml_string> const& str)
{
    return (xaml_std_string_t)to_string_view_t(str);
}

inline xaml_result to_string_t(xaml_ptr<xaml_string> const& str, xaml_std_string_t& s) noexcept
{
    xaml_std_string_view_t view;
    XAML_RETURN_IF_FAILED(to_string_view_t(str, view));
    s = (xaml_std_string_t)view;
    return XAML_S_OK;
}
#endif // __cplusplus

#endif // !XAML_STRING_H
