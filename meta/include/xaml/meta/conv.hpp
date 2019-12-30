#ifndef XAML_CONV_HPP
#define XAML_CONV_HPP

#include <any>
#include <cstdlib>
#include <locale>
#include <string>
#include <typeindex>

namespace xaml
{
    struct value_converter
    {
        virtual std::any convert(std::any value, std::type_index const& target_type, std::any param, std::locale language) = 0;
        virtual std::any convert_back(std::any value, std::type_index const& target_type, std::any param, std::locale language) = 0;
    };

    template <typename T, typename = void>
    struct __value_converter_traits
    {
        static T convert(std::any value)
        {
            if (value.type() == typeid(T))
            {
                return std::any_cast<T>(value);
            }
            return {};
        }
        static std::any convert_back(T value) { return value; }
    };

    template <typename T, typename Traits = __value_converter_traits<T>>
    struct __value_converter_template : public value_converter
    {
        std::any convert(std::any value, std::type_index const& target_type, std::any param, std::locale language) override
        {
            return Traits::convert(value);
        }
        std::any convert_back(std::any value, std::type_index const& target_type, std::any param, std::locale language) override
        {
            if (value.type() == typeid(T))
            {
                return Traits::convert_back(std::any_cast<T>(value));
            }
            return T{};
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
    struct __value_converter_traits<T, std::enable_if_t<__can_stoi_v<T>>>
    {
        static T convert(std::any value)
        {
            if (value.type() == typeid(T))
            {
                return std::any_cast<T>(value);
            }
            else if (value.type() == typeid(std::string))
            {
                return stoi<T>(std::any_cast<std::string>(value));
            }
            else if (value.type() == typeid(std::string_view))
            {
                return stoi<T>(std::any_cast<std::string_view>(value));
            }
            else if (value.type() == typeid(char*) || value.type() == typeid(const char*))
            {
                return stoi<T>(std::any_cast<const char*>(value));
            }
            else if (value.type() == typeid(std::wstring))
            {
                return stoi<T>(std::any_cast<std::wstring>(value));
            }
            else if (value.type() == typeid(std::wstring_view))
            {
                return stoi<T>(std::any_cast<std::wstring_view>(value));
            }
            else if (value.type() == typeid(wchar_t*) || value.type() == typeid(const wchar_t*))
            {
                return stoi<T>(std::any_cast<const wchar_t*>(value));
            }
            else
            {
                return {};
            }
        }
        static std::any convert_back(T value) { return std::to_string(value); }
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
    struct __value_converter_traits<T, std::enable_if_t<__can_stof_v<T>>>
    {
        static T convert(std::any value)
        {
            if (value.type() == typeid(T))
            {
                return std::any_cast<T>(value);
            }
            else if (value.type() == typeid(std::string))
            {
                return stof<T>(std::any_cast<std::string>(value));
            }
            else if (value.type() == typeid(std::string_view))
            {
                return stof<T>(std::any_cast<std::string_view>(value));
            }
            else if (value.type() == typeid(char*) || value.type() == typeid(const char*))
            {
                return stof<T>(std::any_cast<const char*>(value));
            }
            else if (value.type() == typeid(std::wstring))
            {
                return stof<T>(std::any_cast<std::wstring>(value));
            }
            else if (value.type() == typeid(std::wstring_view))
            {
                return stof<T>(std::any_cast<std::wstring_view>(value));
            }
            else if (value.type() == typeid(wchar_t*) || value.type() == typeid(const wchar_t*))
            {
                return stof<T>(std::any_cast<const wchar_t*>(value));
            }
            else
            {
                return {};
            }
        }
        static std::any convert_back(T value) { return std::to_string(value); }
    };
} // namespace xaml

#endif // !XAML_CONV_HPP
