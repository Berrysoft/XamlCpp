#ifndef XAML_CONV_HPP
#define XAML_CONV_HPP

#include <any>
#include <cstdlib>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <typeindex>
#include <xaml/strings.hpp>
#include <xaml/utility.hpp>

namespace xaml
{
    template <typename T, typename = void>
    struct value_converter_traits
    {
    private:
        using return_type = std::decay_t<T>;

    public:
        static return_type convert(std::any value)
        {
            if (value.type() == typeid(return_type))
            {
                return std::any_cast<return_type>(value);
            }
            return {};
        }
    };

    template <typename T, T (*func)(std::string_view), T (*wfunc)(std::wstring_view)>
    struct __value_converter_traits_helper
    {
        static T convert(std::any value)
        {
            if (value.type() == typeid(T))
            {
                return std::any_cast<T>(value);
            }
            else if (value.type() == typeid(std::string))
            {
                return func(std::any_cast<std::string>(value));
            }
            else if (value.type() == typeid(std::string_view))
            {
                return func(std::any_cast<std::string_view>(value));
            }
            else if (value.type() == typeid(char*) || value.type() == typeid(const char*))
            {
                return func(std::any_cast<const char*>(value));
            }
            else if (value.type() == typeid(std::wstring))
            {
                return wfunc(std::any_cast<std::wstring>(value));
            }
            else if (value.type() == typeid(std::wstring_view))
            {
                return wfunc(std::any_cast<std::wstring_view>(value));
            }
            else if (value.type() == typeid(wchar_t*) || value.type() == typeid(const wchar_t*))
            {
                return wfunc(std::any_cast<const wchar_t*>(value));
            }
            else
            {
                return {};
            }
        }
    };

    template <typename TInt, typename TChar>
    struct __stoi_helper;

    template <typename TInt>
    struct __stoi_helper<TInt, char>
    {
        using string_view_type = std::string_view;
        using return_type = TInt;
        return_type operator()(string_view_type str) const noexcept
        {
            long value = std::strtol(str.data(), nullptr, 10);
            return (return_type)value;
        }
    };

    template <>
    struct __stoi_helper<unsigned long, char>
    {
        using string_view_type = std::string_view;
        using return_type = unsigned long;
        return_type operator()(string_view_type str) const noexcept
        {
            unsigned long value = std::strtoul(str.data(), nullptr, 10);
            return value;
        }
    };

    template <>
    struct __stoi_helper<long long, char>
    {
        using string_view_type = std::string_view;
        using return_type = long long;
        return_type operator()(string_view_type str) const noexcept
        {
            long long value = std::strtoll(str.data(), nullptr, 10);
            return value;
        }
    };

    template <>
    struct __stoi_helper<unsigned long long, char>
    {
        using string_view_type = std::string_view;
        using return_type = unsigned long long;
        return_type operator()(string_view_type str) const noexcept
        {
            unsigned long long value = std::strtoull(str.data(), nullptr, 10);
            return value;
        }
    };

    template <typename TInt>
    struct __stoi_helper<TInt, wchar_t>
    {
        using string_view_type = std::wstring_view;
        using return_type = TInt;
        return_type operator()(string_view_type str) const noexcept
        {
            long value = std::wcstol(str.data(), nullptr, 10);
            return (return_type)value;
        }
    };

    template <>
    struct __stoi_helper<unsigned long, wchar_t>
    {
        using string_view_type = std::wstring_view;
        using return_type = unsigned long;
        return_type operator()(string_view_type str) const noexcept
        {
            unsigned long value = std::wcstoul(str.data(), nullptr, 10);
            return value;
        }
    };

    template <>
    struct __stoi_helper<long long, wchar_t>
    {
        using string_view_type = std::wstring_view;
        using return_type = long long;
        return_type operator()(string_view_type str) const noexcept
        {
            long long value = std::wcstoll(str.data(), nullptr, 10);
            return value;
        }
    };

    template <>
    struct __stoi_helper<unsigned long long, wchar_t>
    {
        using string_view_type = std::wstring_view;
        using return_type = unsigned long long;
        return_type operator()(string_view_type str) const noexcept
        {
            unsigned long long value = std::wcstoull(str.data(), nullptr, 10);
            return value;
        }
    };

    template <typename T>
    struct __can_stoi : std::false_type
    {
    };

    template <>
    struct __can_stoi<signed char> : std::true_type
    {
    };

    template <>
    struct __can_stoi<short> : std::true_type
    {
    };

    template <>
    struct __can_stoi<int> : std::true_type
    {
    };

    template <>
    struct __can_stoi<long> : std::true_type
    {
    };

    template <>
    struct __can_stoi<long long> : std::true_type
    {
    };

    template <>
    struct __can_stoi<unsigned char> : std::true_type
    {
    };

    template <>
    struct __can_stoi<unsigned short> : std::true_type
    {
    };

    template <>
    struct __can_stoi<unsigned int> : std::true_type
    {
    };

    template <>
    struct __can_stoi<unsigned long> : std::true_type
    {
    };

    template <>
    struct __can_stoi<unsigned long long> : std::true_type
    {
    };

    template <typename T>
    constexpr bool __can_stoi_v = __can_stoi<T>::value;

    template <typename TInt, typename TChar, typename = std::enable_if_t<__can_stoi_v<TInt>>>
    inline TInt __stoi(std::basic_string_view<TChar> str) noexcept
    {
        return __stoi_helper<TInt, TChar>{}(str);
    }

    template <typename TInt>
    inline TInt stoi(std::string_view str) noexcept
    {
        return __stoi<TInt, char>(str);
    }

    template <typename TInt>
    inline TInt stoi(std::wstring_view str) noexcept
    {
        return __stoi<TInt, wchar_t>(str);
    }

    template <typename T>
    struct value_converter_traits<T, std::enable_if_t<__can_stoi_v<T>>> : __value_converter_traits_helper<T, __stoi<T, char>, __stoi<T, wchar_t>>
    {
    };

    template <typename TFloat, typename TChar>
    struct __stof_helper;

    template <typename TFloat>
    struct __stof_helper<TFloat, char>
    {
        using string_view_type = std::string_view;
        using return_type = TFloat;
        return_type operator()(string_view_type str) const noexcept
        {
            float value = std::strtof(str.data(), nullptr);
            return (return_type)value;
        }
    };

    template <>
    struct __stof_helper<double, char>
    {
        using string_view_type = std::string_view;
        using return_type = double;
        return_type operator()(string_view_type str) const noexcept
        {
            double value = std::strtod(str.data(), nullptr);
            return value;
        }
    };

    template <>
    struct __stof_helper<long double, char>
    {
        using string_view_type = std::string_view;
        using return_type = long double;
        return_type operator()(string_view_type str) const noexcept
        {
            long double value = std::strtold(str.data(), nullptr);
            return value;
        }
    };

    template <typename TFloat>
    struct __stof_helper<TFloat, wchar_t>
    {
        using string_view_type = std::wstring_view;
        using return_type = TFloat;
        return_type operator()(string_view_type str) const noexcept
        {
            float value = std::wcstof(str.data(), nullptr);
            return (return_type)value;
        }
    };

    template <>
    struct __stof_helper<double, wchar_t>
    {
        using string_view_type = std::wstring_view;
        using return_type = double;
        return_type operator()(string_view_type str) const noexcept
        {
            double value = std::wcstod(str.data(), nullptr);
            return value;
        }
    };

    template <>
    struct __stof_helper<long double, wchar_t>
    {
        using string_view_type = std::wstring_view;
        using return_type = long double;
        return_type operator()(string_view_type str) const noexcept
        {
            long double value = std::wcstold(str.data(), nullptr);
            return value;
        }
    };

    template <typename T>
    struct __can_stof : std::false_type
    {
    };

    template <>
    struct __can_stof<float> : std::true_type
    {
    };

    template <>
    struct __can_stof<double> : std::true_type
    {
    };

    template <>
    struct __can_stof<long double> : std::true_type
    {
    };

    template <typename T>
    constexpr bool __can_stof_v = __can_stof<T>::value;

    template <typename TFloat, typename TChar, typename = std::enable_if_t<__can_stof_v<TFloat>>>
    inline TFloat __stof(std::basic_string_view<TChar> str)
    {
        return __stof_helper<TFloat, TChar>{}(str);
    }

    template <typename TFloat>
    inline TFloat stof(std::string_view str)
    {
        return __stof<TFloat, char>(str);
    }

    template <typename TFloat>
    inline TFloat stof(std::wstring_view str)
    {
        return __stof<TFloat, wchar_t>(str);
    }

    template <typename T>
    struct value_converter_traits<T, std::enable_if_t<__can_stof_v<T>>> : __value_converter_traits_helper<T, __stof<T, char>, __stof<T, wchar_t>>
    {
    };

    STRING_CONST(__true_str, "true")
    STRING_CONST(__false_str, "false")

    template <typename TChar>
    inline bool __stob(std::basic_string_view<TChar> str)
    {
        if (str.length() != 4) return false;
        for (std::size_t i = 0; i < 4; i++)
        {
            if (std::tolower(str[i]) != __true_str<TChar>[i]) return false;
        }
        return true;
    }

    inline bool stob(std::string_view str) { return __stob<char>(str); }
    inline bool stob(std::wstring_view str) { return __stob<wchar_t>(str); }

    template <>
    struct value_converter_traits<bool, void> : __value_converter_traits_helper<bool, __stob<char>, __stob<wchar_t>>
    {
    };

    XAML_META_API std::string __wtomb(std::wstring_view str);

    XAML_META_API std::wstring __mbtow(std::string_view str);

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
            else if (value.type() == typeid(std::wstring))
            {
                return std::any_cast<std::wstring>(value);
            }
            else if (value.type() == typeid(std::wstring_view))
            {
                return (std::wstring)std::any_cast<std::wstring_view>(value);
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
    };

    template <typename T>
    struct value_converter_traits<std::optional<T>, void>
    {
        static std::optional<T> convert(std::any value)
        {
            if (value.type() == typeid(std::nullopt_t))
            {
                return std::nullopt;
            }
            else if (value.type() == typeid(std::optional<T>))
            {
                return std::any_cast<std::optional<T>>(value);
            }
            else
            {
                return std::make_optional<T>(value_converter_traits<T>::convert(value));
            }
        }
    };
} // namespace xaml

#endif // !XAML_CONV_HPP
