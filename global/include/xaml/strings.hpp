#ifndef XAML_UI_STRINGS_HPP
#define XAML_UI_STRINGS_HPP

#include <string>
#include <string_view>
#include <xaml/utility.hpp>

#ifndef U
#ifdef UNICODE
#define U(x) L##x
#else
#define U(x) x
#endif // UNICODE
#endif // !U

#ifndef CHAR_CONST
#define CHAR_CONST(name, value)           \
    template <typename TChar>             \
    inline constexpr TChar name{ value }; \
    template <>                           \
    inline constexpr wchar_t name<wchar_t>{ L##value };
#endif // !CHAR_CONST

#ifndef STRING_CONST
#define STRING_CONST(name, value)                                 \
    template <typename TChar>                                     \
    inline constexpr std::basic_string_view<TChar> name{ value }; \
    template <>                                                   \
    inline constexpr std::wstring_view name<wchar_t>{ L##value };
#endif // !STRING_CONST

namespace xaml
{
#ifdef UNICODE
    using char_t = wchar_t;
#else
    using char_t = char;
#endif // UNICODE

    using string_t = std::basic_string<char_t>;
    using string_view_t = std::basic_string_view<char_t>;

    XAML_GLOBAL_API std::string __wtomb(std::wstring_view str);

    XAML_GLOBAL_API std::wstring __mbtow(std::string_view str);
} // namespace xaml

#endif // !XAML_UI_STRINGS_HPP
