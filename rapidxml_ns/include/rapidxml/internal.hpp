#ifndef RAPID_XML_INTERNAL_HPP
#define RAPID_XML_INTERNAL_HPP

#include <algorithm>
#include <cctype>
#include <string_view>

namespace rapidxml::internal
{
    // Compare strings for equality with option to ignore case
    constexpr bool compare(std::string_view p1, std::string_view p2, bool case_sensitive = true)
    {
        if (case_sensitive)
            return p1 == p2;
        else
            return std::equal(p1.begin(), p1.end(), p2.begin(), p2.end(), [](char c1, char c2) { return std::tolower(c1) == std::tolower(c2); });
    }
} // namespace rapidxml::internal

#endif // !RAPID_XML_INTERNAL_HPP
