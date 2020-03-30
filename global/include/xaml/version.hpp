#ifndef XAML_VERSION_HPP
#define XAML_VERSION_HPP

#include <ostream>
#include <utility>

#if __has_include(<version>)
#include <version>
#endif // __has_include(<version>)

#if __has_include(<compare>)
#include <compare>
#endif // __has_include(<compare>)

namespace xaml
{
    struct version
    {
        std::size_t major;
        std::size_t minor;
        std::size_t patch;

#ifdef __cpp_impl_three_way_comparison
        auto operator<=>(version const&) const = default;
#endif // __cpp_impl_three_way_comparison
    };

    inline constexpr version current_version = { XAML_VERSION_MAJOR, XAML_VERSION_MINOR, XAML_VERSION_PATCH };

    template <typename Char, typename Traits>
    inline std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, version const& v)
    {
        return stream << v.major << Char{ '.' } << v.minor << Char{ '.' } << v.patch;
    }

#ifndef __cpp_impl_three_way_comparison
    constexpr bool operator==(version const& lhs, version const& rhs)
    {
        return lhs.major == rhs.major && lhs.minor == rhs.minor && lhs.patch == rhs.patch;
    }
    constexpr bool operator!=(version const& lhs, version const& rhs) { return !(lhs == rhs); }

    constexpr bool operator>(version const& lhs, version const& rhs) { return lhs.major > rhs.major || (lhs.major == rhs.major && (lhs.minor > rhs.minor || (lhs.minor == rhs.minor && lhs.patch > rhs.patch))); }
    constexpr bool operator<(version const& lhs, version const& rhs) { return rhs > lhs; }
    constexpr bool operator>=(version const& lhs, version const& rhs) { return !(lhs < rhs); }
    constexpr bool operator<=(version const& lhs, version const& rhs) { return !(lhs > rhs); }
#endif // !__cpp_impl_three_way_comparison

} // namespace xaml

#endif // !XAML_VERSION_HPP
