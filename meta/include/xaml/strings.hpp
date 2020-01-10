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
#elif defined(UTF16)
#define U(x) u16##x
#else
#define U(x) x
#endif // UNICODE
#endif // !U

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
    template <typename TChar>
    static inline std::string __tomb(std::basic_string_view<TChar> str)
    {
        std::mbstate_t mb{};
        std::string internal(str.size() * 2, '\0');
        auto& f = std::use_facet<std::codecvt<TChar, char, std::mbstate_t>>(std::locale{});
        const TChar* from_next;
        char* to_next;
        f.out(mb, str.data(), str.data() + str.size(), from_next, internal.data(), internal.data() + internal.size(), to_next);
        internal.resize(to_next - &internal.front());
        return internal;
    }

    static inline std::string __wtomb(std::wstring_view str)
    {
        return __tomb<wchar_t>(str);
    }

    static inline std::string __u16tomb(std::u16string_view str)
    {
        return __tomb<char16_t>(str);
    }

    template <typename TChar>
    static inline std::basic_string<TChar> __mbto(std::string_view str)
    {
        std::mbstate_t mb{};
        std::basic_string<TChar> internal(str.size(), L'\0');
        auto& f = std::use_facet<std::codecvt<TChar, char, std::mbstate_t>>(std::locale{});
        const char* from_next;
        TChar* to_next;
        f.in(mb, str.data(), str.data() + str.size(), from_next, internal.data(), internal.data() + internal.size(), to_next);
        internal.resize(to_next - &internal.front());
        return internal;
    }

    static inline std::wstring __mbtow(std::string_view str)
    {
        return __mbto<wchar_t>(str);
    }

    static inline std::u16string __mbtou16(std::string_view str)
    {
        return __mbto<char16_t>(str);
    }

#ifdef UNICODE
    using char_t = wchar_t;
    using string_t = std::wstring;
    using string_view_t = std::wstring_view;
    using stringstream_t = std::wstringstream;
    using istringstream_t = std::wistringstream;
    using ostringstream_t = std::wostringstream;
#elif defined(UTF16)
    using char_t = char16_t;
    using string_t = std::u16string;
    using string_view_t = std::u16string_view;
    using stringstream_t = std::u16stringstream;
    using istringstream_t = std::u16istringstream;
    using ostringstream_t = std::u16ostringstream;
#else
    using char_t = char;
    using string_t = std::string;
    using string_view_t = std::string_view;
    using stringstream_t = std::stringstream;
    using istringstream_t = std::istringstream;
    using ostringstream_t = std::ostringstream;
#endif // UNICODE

    template <>
    struct value_converter_traits<std::string_view, void>
    {
        static std::string convert(std::any value)
        {
            if (value.type() == typeid(std::string))
            {
                return std::any_cast<std::string>(value);
            }
            else if (value.type() == typeid(std::string_view))
            {
                return (std::string)std::any_cast<std::string_view>(value);
            }
            else if (value.type() == typeid(char*) || value.type() == typeid(const char*))
            {
                return std::any_cast<const char*>(value);
            }
            else
            {
                return {};
            }
        }
    };

    template <>
    struct value_converter_traits<std::wstring_view, void>
    {
        static std::wstring convert(std::any value)
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
            else
            {
                return {};
            }
        }
    };

    template <>
    struct value_converter_traits<std::u16string_view, void>
    {
        static std::u16string convert(std::any value)
        {
            if (value.type() == typeid(std::string))
            {
                return __mbtou16(std::any_cast<std::string>(value));
            }
            else if (value.type() == typeid(std::string_view))
            {
                return __mbtou16(std::any_cast<std::string_view>(value));
            }
            else if (value.type() == typeid(char*) || value.type() == typeid(const char*))
            {
                return __mbtou16(std::any_cast<const char*>(value));
            }
            else
            {
                return {};
            }
        }
    };
} // namespace xaml

#endif // !XAML_UI_STRINGS_HPP
