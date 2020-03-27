#ifndef XAML_UI_DRAWING_HPP
#define XAML_UI_DRAWING_HPP

#include <algorithm>
#include <array>
#include <cstdint>
#include <map>
#include <sstream>
#include <tuple>
#include <utility>
#include <xaml/meta/conv.hpp>
#include <xaml/strings.hpp>

namespace xaml
{
    template <typename T, typename TTuple, std::size_t... Indicies>
    T __intialize_from_tuple_impl(TTuple&& t, std::index_sequence<Indicies...>)
    {
        return T{ std::get<Indicies>(std::forward<TTuple>(t))... };
    }

    template <typename T, typename TTuple>
    T __initialize_from_tuple(TTuple&& t)
    {
        return __intialize_from_tuple_impl<T>(std::forward<TTuple>(t), std::make_index_sequence<std::tuple_size_v<TTuple>>{});
    }

    template <typename T, typename TTuple, TTuple (*func)(std::string_view), TTuple (*wfunc)(std::wstring_view)>
    struct __tuple___value_converter_traits_helper
    {
        static TTuple __convert(std::shared_ptr<meta_class> value)
        {
            if (auto box = value->query<meta_box<TTuple>>())
            {
                return *box;
            }
            else if (auto str = value->query<meta_box<std::string>>())
            {
                return func(str->get());
            }
            else if (auto wstr = value->query<meta_box<std::wstring>>())
            {
                return wfunc(wstr->get());
            }
            else
            {
                return {};
            }
        }

        static T convert(std::shared_ptr<meta_class> value)
        {
            if (value)
            {
                if (auto box = value->query<meta_box<T>>())
                {
                    return *box;
                }
                else
                {
                    auto t = __convert(std::move(value));
                    return __initialize_from_tuple<T>(std::move(t));
                }
            }
            return {};
        }
    };

    STRING_CONST(__delimeter, " ,\t\r\n")

    struct size
    {
        double width;
        double height;
    };

    constexpr bool operator==(size lhs, size rhs) { return lhs.width == rhs.width && lhs.height == rhs.height; }
    constexpr bool operator!=(size lhs, size rhs) { return !(lhs == rhs); }

    constexpr size operator+(size lhs, size rhs) { return { lhs.width + rhs.width, lhs.height + rhs.height }; }
    constexpr size operator-(size lhs, size rhs) { return { lhs.width - rhs.width, lhs.height - rhs.height }; }

    constexpr size operator*(size lhs, double rhs) { return { lhs.width * rhs, lhs.height * rhs }; }
    constexpr size operator*(double lhs, size rhs) { return rhs * lhs; }
    constexpr size operator/(size lhs, double rhs) { return lhs * (1 / rhs); }

    template <>
    struct type_guid<meta_box<size>>
    {
        static constexpr guid value{ 0xf7af7a73, 0xaee4, 0x4a8d, { 0x99, 0xff, 0x17, 0x7f, 0xf6, 0xb2, 0xbb, 0xcf } };
    };

    struct point
    {
        double x;
        double y;
    };

    constexpr bool operator==(point lhs, point rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
    constexpr bool operator!=(point lhs, point rhs) { return !(lhs == rhs); }

    constexpr point operator+(point lhs, point rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y }; }
    constexpr point operator-(point lhs, point rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y }; }

    constexpr point operator*(point lhs, double rhs) { return { lhs.x * rhs, lhs.y * rhs }; }
    constexpr point operator*(double lhs, point rhs) { return rhs * lhs; }
    constexpr point operator/(point lhs, double rhs) { return lhs * (1 / rhs); }

    template <>
    struct type_guid<meta_box<point>>
    {
        static constexpr guid value{ 0xd529263c, 0x9ea7, 0x4be9, { 0xa0, 0x81, 0x9a, 0xc1, 0x1e, 0x39, 0x38, 0x9f } };
    };

    template <typename TChar>
    inline std::tuple<double, double> __stot2d(std::basic_string_view<TChar> str)
    {
        std::size_t index = str.find_first_of(__delimeter<TChar>);
        if (index == std::basic_string_view<TChar>::npos)
        {
            double d = stof<double>(str);
            return std::make_tuple(d, d);
        }
        std::size_t index2 = str.find_first_not_of(__delimeter<TChar>, index);
        return std::make_tuple(stof<double>(str.substr(0, index)), stof<double>(str.substr(index2)));
    }

    inline std::tuple<double, double> stot2d(std::string_view str) { return __stot2d<char>(str); }
    inline std::tuple<double, double> stot2d(std::wstring_view str) { return __stot2d<wchar_t>(str); }

    template <>
    struct type_guid<meta_box<std::tuple<double, double>>>
    {
        static constexpr guid value{ 0x59daccd1, 0xd45f, 0x4ffb, { 0xb8, 0x26, 0x7a, 0x76, 0xac, 0x70, 0x26, 0x73 } };
    };

    template <typename T>
    struct __can_stot2d : std::false_type
    {
    };

    template <>
    struct __can_stot2d<size> : std::true_type
    {
    };

    template <>
    struct __can_stot2d<point> : std::true_type
    {
    };

    template <>
    struct __can_stot2d<std::tuple<double, double>> : std::true_type
    {
    };

    template <>
    struct __can_stot2d<std::pair<double, double>> : std::true_type
    {
    };

    template <>
    struct __can_stot2d<std::array<double, 2>> : std::true_type
    {
    };

    template <typename T>
    constexpr bool __can_stot2d_v = __can_stot2d<T>::value;

    template <typename T>
    struct value_converter_traits<T, std::enable_if_t<__can_stot2d_v<T>>> : __tuple___value_converter_traits_helper<T, std::tuple<double, double>, __stot2d<char>, __stot2d<wchar_t>>
    {
    };

    struct rectangle
    {
        double x, y;
        double width, height;
    };

    constexpr bool operator==(rectangle const& lhs, rectangle const& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.width == rhs.width && lhs.height == rhs.height; }
    constexpr bool operator!=(rectangle const& lhs, rectangle const& rhs) { return !(lhs == rhs); }

    constexpr rectangle operator+(point lhs, size rhs) { return { lhs.x, lhs.y, rhs.width, rhs.height }; }

    constexpr rectangle operator*(rectangle const& lhs, double rhs) { return { lhs.x * rhs, lhs.y * rhs, lhs.width * rhs, lhs.height * rhs }; }
    constexpr rectangle operator*(double lhs, rectangle const& rhs) { return rhs * lhs; }
    constexpr rectangle operator/(rectangle const& lhs, double rhs) { return lhs * (1 / rhs); }

    template <>
    struct type_guid<meta_box<rectangle>>
    {
        static constexpr guid value{ 0xf54e02a0, 0xfb1e, 0x4081, { 0xa0, 0xd6, 0xae, 0x72, 0xbc, 0x8c, 0x78, 0x00 } };
    };

    struct margin
    {
        double left;
        double top;
        double right;
        double bottom;
    };

    constexpr bool operator==(margin const& lhs, margin const& rhs) { return lhs.left == rhs.left && lhs.top == rhs.top && lhs.right == rhs.right && lhs.bottom == rhs.bottom; }
    constexpr bool operator!=(margin const& lhs, margin const& rhs) { return !(lhs == rhs); }

    constexpr rectangle operator+(rectangle const& lhs, margin const& rhs) { return { lhs.x - rhs.left, lhs.y - rhs.top, lhs.width + rhs.left + rhs.right, lhs.height + rhs.top + rhs.bottom }; }
    constexpr rectangle operator-(rectangle const& lhs, margin const& rhs) { return { lhs.x + rhs.left, lhs.y + rhs.top, lhs.width - rhs.left - rhs.right, lhs.height - rhs.top - rhs.bottom }; }

    constexpr margin operator*(margin const& lhs, double rhs) { return { lhs.left * rhs, lhs.top * rhs, lhs.right * rhs, lhs.bottom * rhs }; }
    constexpr margin operator*(double lhs, margin const& rhs) { return rhs * lhs; }
    constexpr margin operator/(margin const& lhs, double rhs) { return lhs * (1 / rhs); }

    template <>
    struct type_guid<meta_box<margin>>
    {
        static constexpr guid value{ 0xb31a5b36, 0x30c3, 0x408f, { 0xae, 0x44, 0xaf, 0xc2, 0xb6, 0x65, 0xc7, 0x3e } };
    };

    template <typename TChar>
    inline std::tuple<double, double, double, double> __stot4d(std::basic_string_view<TChar> str)
    {
        std::size_t len1 = str.find_first_of(__delimeter<TChar>);
        if (len1 == std::basic_string_view<TChar>::npos)
        {
            double d = stof<double>(str);
            return std::make_tuple(d, d, d, d);
        }
        std::size_t index2 = str.find_first_not_of(__delimeter<TChar>, len1);
        std::size_t len2 = str.find_first_of(__delimeter<TChar>, index2);
        if (len2 == std::basic_string_view<TChar>::npos)
        {
            double d1 = stof<double>(str.substr(0, len1));
            double d2 = stof<double>(str.substr(index2));
            return std::make_tuple(d1, d2, d1, d2);
        }
        std::size_t index3 = str.find_first_not_of(__delimeter<TChar>, len2);
        std::size_t len3 = str.find_first_of(__delimeter<TChar>, index3);
        std::size_t index4 = str.find_first_not_of(__delimeter<TChar>, len3);
        return std::make_tuple(stof<double>(str.substr(0, len1)), stof<double>(str.substr(index2, len2 - index2)), stof<double>(str.substr(index3, len3 - index3)), stof<double>(str.substr(index4)));
    }

    inline std::tuple<double, double, double, double> stot4d(std::string_view str) { return __stot4d<char>(str); }
    inline std::tuple<double, double, double, double> stot4d(std::wstring_view str) { return __stot4d<wchar_t>(str); }

    template <>
    struct type_guid<meta_box<std::tuple<double, double, double, double>>>
    {
        static constexpr guid value{ 0x9dfdd0ef, 0xe190, 0x44f4, { 0xbd, 0x7b, 0x65, 0xff, 0x1f, 0x20, 0x4a, 0xbf } };
    };

    template <typename T>
    struct __can_stot4d : std::false_type
    {
    };

    template <>
    struct __can_stot4d<rectangle> : std::true_type
    {
    };

    template <>
    struct __can_stot4d<margin> : std::true_type
    {
    };

    template <>
    struct __can_stot4d<std::tuple<double, double, double, double>> : std::true_type
    {
    };

    template <>
    struct __can_stot4d<std::array<double, 4>> : std::true_type
    {
    };

    template <typename T>
    constexpr bool __can_stot4d_v = __can_stot4d<T>::value;

    template <typename T>
    struct value_converter_traits<T, std::enable_if_t<__can_stot4d_v<T>>> : __tuple___value_converter_traits_helper<T, std::tuple<double, double, double, double>, __stot4d<char>, __stot4d<wchar_t>>
    {
    };

    struct alignas(1) color
    {
        std::uint8_t a, r, g, b;
        constexpr operator std::uint32_t() const noexcept { return ((std::uint32_t)a << 24) + ((std::uint32_t)r << 16) + ((std::uint32_t)g << 8) + (std::uint32_t)b; }
        static constexpr color from_argb(std::uint32_t v) noexcept { return { (std::uint8_t)((v >> 24) & 0xFF), (std::uint8_t)((v >> 16) & 0xFF), (std::uint8_t)((v >> 8) & 0xFF), (std::uint8_t)(v & 0xFF) }; }
    };

    constexpr bool operator==(color lhs, color rhs) { return (std::uint32_t)lhs == (std::uint32_t)rhs; }
    constexpr bool operator!=(color lhs, color rhs) { return !(lhs == rhs); }
} // namespace xaml
#endif // !XAML_UI_DRAWING_HPP
