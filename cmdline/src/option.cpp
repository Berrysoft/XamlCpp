#include <xaml/cmdline/option.hpp>

using namespace std;

namespace xaml::cmdline
{
    optional<string_view> option::find_short_arg(char_t name) const
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

    optional<string_view> option::find_long_arg(string_t name) const
    {
        auto it = m_long_args.find(name);
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
        // TODO
    }
} // namespace xaml::cmdline
