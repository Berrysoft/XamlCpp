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
    xaml_result operator()(xaml_ptr<xaml_object> const& obj, return_type* value) const noexcept
    {
        if (obj)
        {
            if (auto box = obj.query<xaml_box<std::decay_t<T>>>())
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
    xaml_result operator()(xaml_ptr<xaml_object> const& obj, T** value) const noexcept
    {
        return obj ? obj->query(value) : XAML_E_INVALIDARG;
    }
};

template <typename T, T (*func)(std::string_view) noexcept>
struct __xaml_converter_helper
{
    xaml_result operator()(xaml_ptr<xaml_object> const& obj, T* value) const noexcept
    {
        if (obj)
        {
            if (auto box = obj.query<xaml_box<T>>())
            {
                return box->get_value(value);
            }
            else if (auto str = obj.query<xaml_string>())
            {
                std::string_view view;
                XAML_RETURN_IF_FAILED(to_string_view(str, &view));
                *value = func(view);
                return XAML_S_OK;
            }
        }
        return XAML_E_INVALIDARG;
    }
};

template <typename TInt>
struct __stoi_helper
{
    using return_type = TInt;
    return_type operator()(std::string_view str) const noexcept
    {
        long value = std::strtol(str.data(), nullptr, 10);
        return static_cast<return_type>(value);
    }
};

template <>
struct __stoi_helper<unsigned long>
{
    using return_type = unsigned long;
    return_type operator()(std::string_view str) const noexcept
    {
        return std::strtoul(str.data(), nullptr, 10);
    }
};

template <>
struct __stoi_helper<long long>
{
    using return_type = long long;
    return_type operator()(std::string_view str) const noexcept
    {
        return std::strtoll(str.data(), nullptr, 10);
    }
};

template <>
struct __stoi_helper<unsigned long long>
{
    using return_type = unsigned long long;
    return_type operator()(std::string_view str) const noexcept
    {
        return std::strtoull(str.data(), nullptr, 10);
    }
};

template <>
struct __stoi_helper<bool>
{
    using return_type = bool;
    return_type operator()(std::string_view str) const noexcept
    {
        constexpr const char __true_str[] = "true";
        if (str.length() != 4) return false;
        for (std::size_t i = 0; i < 4; i++)
        {
            if (std::tolower(str[i]) != __true_str[i]) return false;
        }
        return true;
    }
};

template <>
struct __stoi_helper<char>
{
    using return_type = char;
    return_type operator()(std::string_view str) const noexcept
    {
        if (str.empty())
            return 0;
        else
            return str[0];
    }
};

template <typename T>
constexpr bool __can_stoi_v = std::is_integral_v<T>;

template <typename TInt>
inline TInt __stoi(std::string_view str) noexcept
{
    return __stoi_helper<TInt>{}(str);
}

template <typename T>
struct __xaml_converter<T, std::enable_if_t<__can_stoi_v<T>>> : __xaml_converter_helper<T, __stoi<T>>
{
};

template <typename TFloat>
struct __stof_helper
{
    using return_type = TFloat;
    return_type operator()(std::string_view str) const noexcept
    {
        float value = std::strtold(str.data(), nullptr);
        return static_cast<return_type>(value);
    }
};

template <>
struct __stof_helper<float>
{
    using return_type = float;
    return_type operator()(std::string_view str) const noexcept
    {
        return std::strtof(str.data(), nullptr);
    }
};

template <>
struct __stof_helper<double>
{
    using return_type = double;
    return_type operator()(std::string_view str) const noexcept
    {
        return std::strtod(str.data(), nullptr);
    }
};

template <typename T>
constexpr bool __can_stof_v = std::is_floating_point_v<T>;

template <typename TFloat>
inline TFloat __stof(std::string_view str) noexcept
{
    return __stof_helper<TFloat>{}(str);
}

template <typename T>
struct __xaml_converter<T, std::enable_if_t<__can_stof_v<T>>> : __xaml_converter_helper<T, __stof<T>>
{
};

#endif // !XAML_CONV_HPP
