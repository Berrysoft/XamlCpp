#include <xaml/cmdline/option.hpp>

using namespace std;

namespace xaml::cmdline
{
    optional<string_view> option::find_short_arg(char_t name) const noexcept
    {
        auto it = m_short_args.find(name);
        if (it != m_short_args.end())
        {
            return m_properties.at(it->second);
        }
        else
        {
            return nullopt;
        }
    }

    optional<string_view> option::find_long_arg(string_view_t name) const noexcept
    {
        auto it = m_long_args.find((string_t)name);
        if (it != m_long_args.end())
        {
            return m_properties.at(it->second);
        }
        else
        {
            return nullopt;
        }
    }

    void option::add_arg(optional<char_t> short_name, optional<string_view_t> long_name, string_view prop_name)
    {
        if (short_name || long_name)
        {
            auto key = hash<string_view>{}(prop_name);
            m_properties.emplace(key, prop_name);
            if (short_name) m_short_args.emplace(*short_name, key);
            if (long_name) m_long_args.emplace(*long_name, key);
        }
        else
        {
            m_default_property = prop_name;
        }
    }
} // namespace xaml::cmdline
