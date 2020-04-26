#ifndef XAML_UI_DRAWING_H
#define XAML_UI_DRAWING_H

#ifdef __cplusplus
#include <algorithm>
#include <cstdint>
#include <tuple>
#include <version>
#include <xaml/meta/conv.hpp>
#if __has_include(<compare>)
#include <compare>
#endif // __has_include(<compare>)
#else
#include <stdint.h>
#endif // __cplusplus

#ifdef __cplusplus
template <typename T, typename TTuple, std::int32_t... Indicies>
T __intialize_from_tuple_impl(TTuple&& t, std::index_sequence<Indicies...>) noexcept
{
    return T{ std::get<Indicies>(std::forward<TTuple>(t))... };
}

template <typename T, typename TTuple>
T __initialize_from_tuple(TTuple&& t) noexcept
{
    return __intialize_from_tuple_impl<T>(std::forward<TTuple>(t), std::make_index_sequence<std::tuple_size_v<TTuple>>{});
}

template <typename T, typename TTuple, TTuple (*func)(xaml_std_string_view_t) noexcept>
struct __xaml_tuple_converter_helper
{
    xaml_result __convert(xaml_ptr<xaml_object> const& obj, TTuple& value) const noexcept
    {
        if (auto str = obj.query<xaml_string>())
        {
            xaml_std_string_view_t view;
            XAML_RETURN_IF_FAILED(to_string_view_t(str, view));
            value = func(view);
            return XAML_S_OK;
        }
        return XAML_E_INVALIDARG;
    }

    xaml_result operator()(xaml_ptr<xaml_object> const& obj, T& value) const noexcept
    {
        if (obj)
        {
            if (auto box = obj.query<xaml_box>())
            {
                return box->get_value(value);
            }
            else
            {
                TTuple t;
                XAML_RETURN_IF_FAILED(__convert(obj, t));
                value = __initialize_from_tuple<T>(std::move(t));
                return XAML_S_OK;
            }
        }
        return XAML_E_INVALIDARG;
    }
};
#endif // __cplusplus

typedef struct xaml_size xaml_size;

struct xaml_size
{
    double width;
    double height;

#ifdef __cpp_impl_three_way_comparison
    auto operator<=>(xaml_size const&) const = default;
#endif // __cpp_impl_three_way_comparison
};

#ifdef __cplusplus
#ifndef __cpp_impl_three_way_comparison
constexpr bool operator==(xaml_size lhs, xaml_size rhs)
{
    return lhs.width == rhs.width && lhs.height == rhs.height;
}
constexpr bool operator!=(xaml_size lhs, xaml_size rhs) { return !(lhs == rhs); }
#endif // !__cpp_impl_three_way_comparison

constexpr xaml_size operator+(xaml_size lhs, xaml_size rhs)
{
    return { lhs.width + rhs.width, lhs.height + rhs.height };
}
constexpr xaml_size operator-(xaml_size lhs, xaml_size rhs) { return { lhs.width - rhs.width, lhs.height - rhs.height }; }

constexpr xaml_size operator*(xaml_size lhs, double rhs) { return { lhs.width * rhs, lhs.height * rhs }; }
constexpr xaml_size operator*(double lhs, xaml_size rhs) { return rhs * lhs; }
constexpr xaml_size operator/(xaml_size lhs, double rhs) { return lhs * (1 / rhs); }
#endif // __cplusplus

XAML_TYPE(xaml_size, { 0xf7af7a73, 0xaee4, 0x4a8d, { 0x99, 0xff, 0x17, 0x7f, 0xf6, 0xb2, 0xbb, 0xcf } })

typedef struct xaml_point xaml_point;

struct xaml_point
{
    double x;
    double y;

#ifdef __cpp_impl_three_way_comparison
    auto operator<=>(xaml_point const&) const = default;
#endif // __cpp_impl_three_way_comparison
};

#ifdef __cplusplus
#ifndef __cpp_impl_three_way_comparison
constexpr bool operator==(xaml_point lhs, xaml_point rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
constexpr bool operator!=(xaml_point lhs, xaml_point rhs) { return !(lhs == rhs); }
#endif // !__cpp_impl_three_way_comparison

constexpr xaml_point operator+(xaml_point lhs, xaml_point rhs)
{
    return { lhs.x + rhs.x, lhs.y + rhs.y };
}
constexpr xaml_point operator-(xaml_point lhs, xaml_point rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y }; }

constexpr xaml_point operator*(xaml_point lhs, double rhs) { return { lhs.x * rhs, lhs.y * rhs }; }
constexpr xaml_point operator*(double lhs, xaml_point rhs) { return rhs * lhs; }
constexpr xaml_point operator/(xaml_point lhs, double rhs) { return lhs * (1 / rhs); }
#endif // __cplusplus

XAML_TYPE(xaml_point, { 0xd529263c, 0x9ea7, 0x4be9, { 0xa0, 0x81, 0x9a, 0xc1, 0x1e, 0x39, 0x38, 0x9f } })

#ifdef __cplusplus
template <typename TChar>
inline std::tuple<double, double> __stot2d(std::basic_string_view<TChar> str) noexcept
{
    constexpr TChar __delimeter[] = { ' ', ',', '\t', '\r', '\n', '\0' };
    std::int32_t index = str.find_first_of(__delimeter);
    if (index == std::basic_string_view<TChar>::npos)
    {
        double d = __stof<double, TChar>(str);
        return std::make_tuple(d, d);
    }
    std::int32_t index2 = str.find_first_not_of(__delimeter, index);
    return std::make_tuple(__stof<double, TChar>(str.substr(0, index)), __stof<double, TChar>(str.substr(index2)));
}

template <typename T>
struct __can_stot2d : std::false_type
{
};

template <>
struct __can_stot2d<xaml_size> : std::true_type
{
};

template <>
struct __can_stot2d<xaml_point> : std::true_type
{
};

template <typename T>
inline constexpr bool __can_stot2d_v = __can_stot2d<T>::value;

template <typename T>
struct __xaml_converter<T, std::enable_if_t<__can_stot2d_v<T>>> : __xaml_tuple_converter_helper<T, std::tuple<double, double>, __stot2d<xaml_char_t>>
{
};
#endif // __cplusplus

struct xaml_rectangle
{
    double x, y;
    double width, height;

#ifdef __cpp_impl_three_way_comparison
    auto operator<=>(xaml_rectangle const&) const = default;
#endif // __cpp_impl_three_way_comparison
};

#ifdef __cplusplus
#ifndef __cpp_impl_three_way_comparison
constexpr bool operator==(xaml_rectangle const& lhs, xaml_rectangle const& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.width == rhs.width && lhs.height == rhs.height;
}
constexpr bool operator!=(xaml_rectangle const& lhs, xaml_rectangle const& rhs) { return !(lhs == rhs); }
#endif // !__cpp_impl_three_way_comparison

constexpr xaml_rectangle operator+(xaml_point lhs, xaml_size rhs)
{
    return { lhs.x, lhs.y, rhs.width, rhs.height };
}

constexpr xaml_rectangle operator*(xaml_rectangle const& lhs, double rhs) { return { lhs.x * rhs, lhs.y * rhs, lhs.width * rhs, lhs.height * rhs }; }
constexpr xaml_rectangle operator*(double lhs, xaml_rectangle const& rhs) { return rhs * lhs; }
constexpr xaml_rectangle operator/(xaml_rectangle const& lhs, double rhs) { return lhs * (1 / rhs); }
#endif // __cplusplus

XAML_TYPE(xaml_rectangle, { 0xf54e02a0, 0xfb1e, 0x4081, { 0xa0, 0xd6, 0xae, 0x72, 0xbc, 0x8c, 0x78, 0x00 } })

struct xaml_margin
{
    double left;
    double top;
    double right;
    double bottom;

#ifdef __cpp_impl_three_way_comparison
    auto operator<=>(xaml_margin const&) const = default;
#endif // __cpp_impl_three_way_comparison
};

#ifdef __cplusplus
#ifndef __cpp_impl_three_way_comparison
constexpr bool operator==(xaml_margin const& lhs, xaml_margin const& rhs)
{
    return lhs.left == rhs.left && lhs.top == rhs.top && lhs.right == rhs.right && lhs.bottom == rhs.bottom;
}
constexpr bool operator!=(xaml_margin const& lhs, xaml_margin const& rhs) { return !(lhs == rhs); }
#endif // !__cpp_impl_three_way_comparison

constexpr xaml_rectangle operator+(xaml_rectangle const& lhs, xaml_margin const& rhs)
{
    return { lhs.x - rhs.left, lhs.y - rhs.top, lhs.width + rhs.left + rhs.right, lhs.height + rhs.top + rhs.bottom };
}
constexpr xaml_rectangle operator-(xaml_rectangle const& lhs, xaml_margin const& rhs) { return { lhs.x + rhs.left, lhs.y + rhs.top, lhs.width - rhs.left - rhs.right, lhs.height - rhs.top - rhs.bottom }; }

constexpr xaml_margin operator*(xaml_margin const& lhs, double rhs) { return { lhs.left * rhs, lhs.top * rhs, lhs.right * rhs, lhs.bottom * rhs }; }
constexpr xaml_margin operator*(double lhs, xaml_margin const& rhs) { return rhs * lhs; }
constexpr xaml_margin operator/(xaml_margin const& lhs, double rhs) { return lhs * (1 / rhs); }
#endif // __cplusplus

XAML_TYPE(xaml_margin, { 0xb31a5b36, 0x30c3, 0x408f, { 0xae, 0x44, 0xaf, 0xc2, 0xb6, 0x65, 0xc7, 0x3e } })

#ifdef __cplusplus
template <typename TChar>
inline std::tuple<double, double, double, double> __stot4d(std::basic_string_view<TChar> str) noexcept
{
    constexpr TChar __delimeter[] = { ' ', ',', '\t', '\r', '\n', '\0' };
    std::int32_t len1 = str.find_first_of(__delimeter);
    if (len1 == std::basic_string_view<TChar>::npos)
    {
        double d = __stof<double, TChar>(str);
        return std::make_tuple(d, d, d, d);
    }
    std::int32_t index2 = str.find_first_not_of(__delimeter, len1);
    std::int32_t len2 = str.find_first_of(__delimeter, index2);
    if (len2 == std::basic_string_view<TChar>::npos)
    {
        double d1 = __stof<double, TChar>(str.substr(0, len1));
        double d2 = __stof<double, TChar>(str.substr(index2));
        return std::make_tuple(d1, d2, d1, d2);
    }
    std::int32_t index3 = str.find_first_not_of(__delimeter, len2);
    std::int32_t len3 = str.find_first_of(__delimeter, index3);
    std::int32_t index4 = str.find_first_not_of(__delimeter, len3);
    return std::make_tuple(__stof<double, TChar>(str.substr(0, len1)), __stof<double, TChar>(str.substr(index2, len2 - index2)), __stof<double, TChar>(str.substr(index3, len3 - index3)), __stof<double, TChar>(str.substr(index4)));
}

template <typename T>
struct __can_stot4d : std::false_type
{
};

template <>
struct __can_stot4d<xaml_rectangle> : std::true_type
{
};

template <>
struct __can_stot4d<xaml_margin> : std::true_type
{
};

template <typename T>
constexpr bool __can_stot4d_v = __can_stot4d<T>::value;

template <typename T>
struct __xaml_converter<T, std::enable_if_t<__can_stot4d_v<T>>> : __xaml_tuple_converter_helper<T, std::tuple<double, double, double, double>, __stot4d<xaml_char_t>>
{
};
#endif // __cplusplus

typedef struct xaml_color xaml_color;

struct xaml_color
{
    XAML_CSTD uint8_t a;
    XAML_CSTD uint8_t r;
    XAML_CSTD uint8_t g;
    XAML_CSTD uint8_t b;

#ifdef __cplusplus
    constexpr operator std::uint32_t() const noexcept
    {
        return ((std::uint32_t)a << 24) + ((std::uint32_t)r << 16) + ((std::uint32_t)g << 8) + (std::uint32_t)b;
    }
    static constexpr xaml_color from_argb(std::uint32_t v) noexcept { return { (std::uint8_t)((v >> 24) & 0xFF), (std::uint8_t)((v >> 16) & 0xFF), (std::uint8_t)((v >> 8) & 0xFF), (std::uint8_t)(v & 0xFF) }; }

#ifdef __cpp_impl_three_way_comparison
    auto operator<=>(xaml_color const&) const = default;
#endif // __cpp_impl_three_way_comparison
#endif // __cplusplus
};

#ifndef __cpp_impl_three_way_comparison
constexpr bool operator==(xaml_color lhs, xaml_color rhs)
{
    return (std::uint32_t)lhs == (std::uint32_t)rhs;
}
constexpr bool operator!=(xaml_color lhs, xaml_color rhs) { return !(lhs == rhs); }
#endif // !__cpp_impl_three_way_comparison

#endif // !XAML_UI_DRAWING_H
