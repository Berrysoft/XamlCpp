#ifndef XAML_VERSION_HPP
#define XAML_VERSION_HPP

#include <ostream>
#include <utility>

namespace xaml
{
    struct version
    {
        std::size_t major;
        std::size_t minor;
        std::size_t patch;
    };

    inline constexpr version current_version = { XAML_VERSION_MAJOR, XAML_VERSION_MINOR, XAML_VERSION_PATCH };

    template <typename Char, typename Traits>
    inline std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& stream, version const& v)
    {
        return stream << v.major << Char{ '.' } << v.minor << Char{ '.' } << v.patch;
    }

    constexpr bool operator==(version const& lhs, version const& rhs) { return lhs.major == rhs.major && lhs.minor == rhs.minor && lhs.patch == rhs.patch; }
    constexpr bool operator!=(version const& lhs, version const& rhs) { return !(lhs == rhs); }

    constexpr bool operator>(version const& lhs, version const& rhs) { return lhs.major > rhs.major || (lhs.major == rhs.major && (lhs.minor > rhs.minor || (lhs.minor == rhs.minor && lhs.patch > rhs.patch))); }
    constexpr bool operator<(version const& lhs, version const& rhs) { return rhs > lhs; }
    constexpr bool operator>=(version const& lhs, version const& rhs) { return !(lhs < rhs); }
    constexpr bool operator<=(version const& lhs, version const& rhs) { return !(lhs > rhs); }
} // namespace xaml

#endif // !XAML_VERSION_HPP
