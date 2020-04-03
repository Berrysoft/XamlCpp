#ifndef XAML_UI_STRINGS_HPP
#define XAML_UI_STRINGS_HPP

#include <string>
#include <string_view>
#include <xaml/utility.hpp>

#ifndef CHAR_CONST
#define CHAR_CONST(name, value)                         \
    template <typename TChar>                           \
    inline constexpr TChar name{ value };               \
    template <>                                         \
    inline constexpr wchar_t name<wchar_t>{ L##value }; \
    template <>                                         \
    inline constexpr char16_t name<char16_t>{ u##value };
#endif // !CHAR_CONST

#ifndef STRING_CONST
#define STRING_CONST(name, value)                                 \
    template <typename TChar>                                     \
    inline constexpr std::basic_string_view<TChar> name{ value }; \
    template <>                                                   \
    inline constexpr std::wstring_view name<wchar_t>{ L##value }; \
    template <>                                                   \
    inline constexpr std::u16string_view name<char16_t>{ u##value };
#endif // !STRING_CONST

namespace xaml
{
#ifdef UNICODE
    using char_t = wchar_t;
    using uchar_t = wchar_t;

#ifndef U
#define U(x) L##x
#endif // !U

#ifndef UU
#define UU(x) L##x
#endif // !UU

#ifndef to_string_t
#define to_string_t to_wstring
#endif // !to_string_t

#ifndef to_ustring_t
#define to_ustring_t to_wstring
#endif // !to_ustring_t
#elif defined(UTF16UNIX)
    using char_t = char16_t;
    using uchar_t = char;

#ifndef U
#define U(x) u##x
#endif // !U

#ifndef UU
#define UU(x) x
#endif // !UU

#ifndef to_string_t
#define to_string_t to_u16string
#endif // !to_string_t

#ifndef to_ustring_t
#define to_ustring_t to_string
#endif // !to_ustring_t
#else
    using char_t = char;
    using uchar_t = char;

#ifndef U
#define U(x) x
#endif // !U

#ifndef UU
#define UU(x) x
#endif // !UU

#ifndef to_string_t
#define to_string_t to_string
#endif // !to_string_t

#ifndef to_ustring_t
#define to_ustring_t to_string
#endif // !to_ustring_t
#endif // UNICODE

    using string_t = std::basic_string<char_t>;
    using string_view_t = std::basic_string_view<char_t>;

    using ustring_t = std::basic_string<uchar_t>;
    using ustring_view_t = std::basic_string_view<uchar_t>;

    XAML_GLOBAL_API std::string to_string(std::wstring_view str);
    XAML_GLOBAL_API std::string to_string(std::u16string_view str);
    inline std::string to_string(std::string_view str) { return (std::string)str; }

    XAML_GLOBAL_API std::wstring to_wstring(std::string_view str);
    inline std::wstring to_wstring(std::wstring_view str) { return (std::wstring)str; }

    XAML_GLOBAL_API std::u16string to_u16string(std::string_view str);
    inline std::u16string to_u16string(std::u16string_view str) { return (std::u16string)str; }
} // namespace xaml

#endif // !XAML_UI_STRINGS_HPP
