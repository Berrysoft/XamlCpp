#ifndef XAML_STRING_HPP
#define XAML_STRING_HPP

#include <string>
#include <string_view>
#include <xaml/result.hpp>
#include <xaml/string.h>
#include <xaml/xaml_ptr.hpp>

typedef std::basic_string<xaml_char_t> xaml_std_string_t;
typedef std::basic_string_view<xaml_char_t> xaml_std_string_view_t;

XAML_API xaml_result xaml_string_new(xaml_std_string_t&&, xaml_string**) noexcept;
XAML_API xaml_result xaml_string_new(xaml_std_string_view_t, xaml_string**) noexcept;

inline xaml_std_string_view_t to_string_view_t(xaml_ptr<xaml_string> const& str)
{
    xaml_char_t const* data;
    XAML_THROW_IF_FAILED(str->get_data(&data));
    size_t length;
    XAML_THROW_IF_FAILED(str->get_length(&length));
    return xaml_std_string_view_t(data, length);
}

inline xaml_std_string_t to_string_t(xaml_ptr<xaml_string> const& str)
{
    return (xaml_std_string_t)to_string_view_t(str);
}

#endif // !XAML_STRING_HPP
