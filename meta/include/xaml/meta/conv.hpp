#ifndef XAML_CONV_HPP
#define XAML_CONV_HPP

#include <xaml/box.h>
#include <xaml/object.h>
#include <xaml/ptr.hpp>
#include <xaml/string.h>

template <typename T, typename = void>
struct __xaml_converter
{
private:
    using return_type = std::decay_t<T>;

public:
    xaml_result operator()(xaml_ptr<xaml_object> const& obj, return_type& value) const noexcept
    {
        if (obj)
        {
            if (auto box = obj.query<xaml_box>())
            {
                return box->get_value(value);
            }
        }
        return XAML_E_INVALIDARG;
    }
};

template <typename T>
struct __xaml_converter<xaml_ptr<T>, std::enable_if_t<std::is_base_of_v<xaml_object, T>>>
{
    xaml_result operator()(xaml_ptr<xaml_object> const& obj, xaml_ptr<T>& value) const noexcept
    {
        return obj ? obj->query(&value) : XAML_E_INVALIDARG;
    }
};

template <typename T, T (*func)(xaml_std_string_view_t) noexcept>
struct __xaml_converter_helper
{
    xaml_result operator()(xaml_ptr<xaml_object> const& obj, T& value) const noexcept
    {
        if (obj)
        {
            if (auto box = obj.query<xaml_box>())
            {
                return box->get_value(value);
            }
            else if (auto str = obj.query<xaml_string>())
            {
                xaml_std_string_view_t view;
                XAML_RETURN_IF_FAILED(to_string_view_t(str, view));
                value = func(view);
                return XAML_S_OK;
            }
        }
        return XAML_E_INVALIDARG;
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

template <typename T>
struct __xaml_converter<T, std::enable_if_t<__can_stoi_v<T>>> : __xaml_converter_helper<T, __stoi<T, xaml_char_t>>
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
inline TFloat __stof(std::basic_string_view<TChar> str) noexcept
{
    return __stof_helper<TFloat, TChar>{}(str);
}

template <typename T>
struct __xaml_converter<T, std::enable_if_t<__can_stof_v<T>>> : __xaml_converter_helper<T, __stof<T, xaml_char_t>>
{
};

template <typename TChar>
inline bool __stob(std::basic_string_view<TChar> str) noexcept
{
    constexpr TChar __true_str[] = { 't', 'r', 'u', 'e' };
    if (str.length() != 4) return false;
    for (std::size_t i = 0; i < 4; i++)
    {
        if (std::tolower(str[i]) != __true_str[i]) return false;
    }
    return true;
}

template <>
struct __xaml_converter<bool, void> : __xaml_converter_helper<bool, __stob<xaml_char_t>>
{
};

#endif // !XAML_CONV_HPP
