#ifndef XAML_UI_STRINGS_HPP
#define XAML_UI_STRINGS_HPP

#include <locale>
#include <sstream>
#include <string>
#include <string_view>
#include <xaml/meta/conv.hpp>

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
    static inline std::string __wtomb(std::wstring_view str)
    {
        std::mbstate_t mb{};
        std::string internal(str.size() * 2, '\0');
        auto& f = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(std::locale{});
        const wchar_t* from_next;
        char* to_next;
        f.out(mb, str.data(), str.data() + str.size(), from_next, internal.data(), internal.data() + internal.size(), to_next);
        internal.resize(to_next - &internal.front());
        return internal;
    }

    static inline std::wstring __mbtow(std::string_view str)
    {
        std::mbstate_t mb{};
        std::wstring internal(str.size(), L'\0');
        auto& f = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(std::locale{});
        const char* from_next;
        wchar_t* to_next;
        f.in(mb, str.data(), str.data() + str.size(), from_next, internal.data(), internal.data() + internal.size(), to_next);
        internal.resize(to_next - &internal.front());
        return internal;
    }

#ifdef UNICODE
    using char_t = wchar_t;
    using string_t = std::wstring;
    using string_view_t = std::wstring_view;
    using stringstream_t = std::wstringstream;
    using istringstream_t = std::wistringstream;
    using ostringstream_t = std::wostringstream;

    template <>
    struct __value_converter_traits<string_view_t, std::enable_if_t<std::is_same_v<char_t, wchar_t>>>
    {
        static string_t convert(std::any value)
        {
            if (value.type() == typeid(std::string))
            {
                return __mbtow(std::any_cast<std::string>(value));
            }
            else if (value.type() == typeid(std::string_view))
            {
                return __mbtow(std::any_cast<std::string_view>(value));
            }
            else if (value.type() == typeid(char*) || value.type() == typeid(const char*))
            {
                return __mbtow(std::any_cast<const char*>(value));
            }
            else if (value.type() == typeid(std::wstring))
            {
                return std::any_cast<std::wstring>(value);
            }
            else if (value.type() == typeid(std::wstring_view))
            {
                return (string_t)std::any_cast<std::wstring_view>(value);
            }
            else if (value.type() == typeid(wchar_t*) || value.type() == typeid(const wchar_t*))
            {
                return std::any_cast<const wchar_t*>(value);
            }
            else
            {
                return {};
            }
        }
        static std::any convert_back(string_view_t value) { return value; }
    };
#else
    using char_t = char;
    using string_t = std::string;
    using string_view_t = std::string_view;
    using stringstream_t = std::stringstream;
    using istringstream_t = std::istringstream;
    using ostringstream_t = std::ostringstream;

    template <>
    struct __value_converter_traits<string_view_t, std::enable_if_t<std::is_same_v<char_t, char>>>
    {
        static string_t convert(std::any value)
        {
            if (value.type() == typeid(std::string))
            {
                return std::any_cast<std::string>(value);
            }
            else if (value.type() == typeid(std::string_view))
            {
                return (string_t)std::any_cast<std::string_view>(value);
            }
            else if (value.type() == typeid(char*) || value.type() == typeid(const char*))
            {
                return std::any_cast<const char*>(value);
            }
            else if (value.type() == typeid(std::wstring))
            {
                return __wtomb(std::any_cast<std::wstring>(value));
            }
            else if (value.type() == typeid(std::wstring_view))
            {
                return __wtomb(std::any_cast<std::wstring_view>(value));
            }
            else if (value.type() == typeid(wchar_t*) || value.type() == typeid(const wchar_t*))
            {
                return __wtomb(std::any_cast<const wchar_t*>(value));
            }
            else
            {
                return {};
            }
        }
        static std::any convert_back(string_view_t value) { return value; }
    };
#endif // UNICODE
} // namespace xaml

#endif // !XAML_UI_STRINGS_HPP
