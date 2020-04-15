#ifndef XAML_STRING_HPP
#define XAML_STRING_HPP

#include <string>
#include <string_view>
#include <xaml/string.h>

typedef std::basic_string<xaml_char_t> xaml_std_string_t;
typedef std::basic_string_view<xaml_char_t> xaml_std_string_view_t;

XAML_API xaml_result xaml_string_new(xaml_std_string_t&&, xaml_string**) noexcept;
XAML_API xaml_result xaml_string_new(xaml_std_string_view_t, xaml_string**) noexcept;

#endif // !XAML_STRING_HPP
