#ifndef XAML_CMDLINE_OPTION_HPP
#define XAML_CMDLINE_OPTION_HPP

#include <map>
#include <optional>
#include <string>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/strings.hpp>

namespace xaml::cmdline
{
    class option : public meta_class
    {
    private:
        std::map<char_t, size_t> m_short_args;
        std::map<string_t, size_t> m_long_args;
        std::map<size_t, std::string> m_properties;

    public:
        XAML_CMDLINE_API std::optional<std::string_view> find_short_arg(char_t name) const;
        XAML_CMDLINE_API std::optional<std::string_view> find_long_arg(string_t name) const;

        XAML_CMDLINE_API void add_arg(std::optional<char_t> short_name, std::optional<string_view_t> long_name, std::string_view prop_name);
    };
} // namespace xaml::cmdline

#endif // !XAML_CMDLINE_OPTION_HPP
