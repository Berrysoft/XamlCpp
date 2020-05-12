#ifndef XAML_VERSION_HPP
#define XAML_VERSION_HPP

#ifdef __cplusplus
#include <compare>
#include <cstddef>
#include <ostream>
#include <utility>
#else
#include <stdbool.h>
#include <stddef.h>
#endif // __cplusplus

#include <xaml/utility.h>

typedef struct xaml_version xaml_version;

struct xaml_version
{
    XAML_CSTD int32_t major;
    XAML_CSTD int32_t minor;
    XAML_CSTD int32_t patch;

#ifdef __cplusplus
    auto operator<=>(xaml_version const&) const = default;
#endif // __cplusplus
};

XAML_CONSTEXPR_VAR xaml_version xaml_version_current = { XAML_VERSION_MAJOR, XAML_VERSION_MINOR, XAML_VERSION_PATCH };

#ifdef __cplusplus
template <typename Char, typename Traits>
inline std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, xaml_version const& v)
{
    return stream << v.major << Char{ '.' } << v.minor << Char{ '.' } << v.patch;
}
#else
XAML_CONSTEXPR bool xaml_xaml_version_equal(xaml_version const* lhs, xaml_version const* rhs)
{
    return lhs->major == rhs->major && lhs->minor == rhs->minor && lhs->patch == rhs->patch;
}
#endif // __cplusplus

#endif // !XAML_VERSION_HPP
