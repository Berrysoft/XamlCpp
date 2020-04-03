#ifndef XAML_UI_STRINGS_HPP
#define XAML_UI_STRINGS_HPP

#include <codecvt>
#include <locale>
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

#ifndef to_string_t
#define to_string_t to_wstring
#endif // !to_string_t
#else
    using char_t = char;

#ifndef to_string_t
#define to_string_t to_string
#endif // !to_string_t
#endif // UNICODE

    using string_t = std::basic_string<char_t>;
    using string_view_t = std::basic_string_view<char_t>;

#if defined(WIN32) || defined(__MINGW32__)
    using __codecvt_utf8_wchar = std::codecvt_utf8_utf16<wchar_t>;
#else
    using __codecvt_utf8_wchar = std::codecvt_utf8<wchar_t>;
#endif // WIN32 || __MINGW32__

    inline std::string to_string(std::wstring_view str)
    {
        std::wstring_convert<__codecvt_utf8_wchar, wchar_t> wconv;
        return wconv.to_bytes(str.data(), str.data() + str.size());
    }

    inline std::string to_string(std::string_view str) { return (std::string)str; }

    inline std::wstring to_wstring(std::string_view str)
    {
        std::wstring_convert<__codecvt_utf8_wchar, wchar_t> wconv;
        return wconv.from_bytes(str.data(), str.data() + str.size());
    }

    inline std::wstring to_wstring(std::wstring_view str) { return (std::wstring)str; }
} // namespace xaml

#endif // !XAML_UI_STRINGS_HPP
