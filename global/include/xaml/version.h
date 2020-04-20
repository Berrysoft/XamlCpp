#ifndef XAML_VERSION_HPP
#define XAML_VERSION_HPP

#ifdef __cplusplus
#include <cstddef>
#include <ostream>
#include <utility>
#include <version>
#if __has_include(<compare>)
#include <compare>
#endif // __has_include(<compare>)
#else
#include <stddef.h>
#endif // __cplusplus

#include <xaml/utility.h>

typedef struct xaml_version xaml_version;

struct xaml_version
{
    XAML_CSTD size_t major;
    XAML_CSTD size_t minor;
    XAML_CSTD size_t patch;

#ifdef __cpp_impl_three_way_comparison
    auto operator<=>(xaml_version const&) const = default;
#endif // __cpp_impl_three_way_comparison
};

XAML_CONSTEXPR_VAR xaml_version current_xaml_version = { XAML_VERSION_MAJOR, XAML_VERSION_MINOR, XAML_VERSION_PATCH };

#ifdef __cplusplus
template <typename Char, typename Traits>
inline std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, xaml_version const& v)
{
    return stream << v.major << Char{ '.' } << v.minor << Char{ '.' } << v.patch;
}

#ifndef __cpp_impl_three_way_comparison
constexpr bool operator==(xaml_version const& lhs, xaml_version const& rhs)
{
    return lhs.major == rhs.major && lhs.minor == rhs.minor && lhs.patch == rhs.patch;
}
constexpr bool operator!=(xaml_version const& lhs, xaml_version const& rhs) { return !(lhs == rhs); }

constexpr bool operator>(xaml_version const& lhs, xaml_version const& rhs) { return lhs.major > rhs.major || (lhs.major == rhs.major && (lhs.minor > rhs.minor || (lhs.minor == rhs.minor && lhs.patch > rhs.patch))); }
constexpr bool operator<(xaml_version const& lhs, xaml_version const& rhs) { return rhs > lhs; }
constexpr bool operator>=(xaml_version const& lhs, xaml_version const& rhs) { return !(lhs < rhs); }
constexpr bool operator<=(xaml_version const& lhs, xaml_version const& rhs) { return !(lhs > rhs); }
#endif // !__cpp_impl_three_way_comparison
#else
XAML_CONSTEXPR bool xaml_xaml_version_equal(xaml_version const* lhs, xaml_version const* rhs)
{
    return lhs->major == rhs->major && lhs->minor == rhs->minor && lhs->patch == rhs->patch;
}
#endif // __cplusplus

#endif // !XAML_VERSION_HPP
