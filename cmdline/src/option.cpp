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

    shared_ptr<option> option::add_arg(char_t short_name, string_view_t long_name, string_view prop_name, string_view_t help_text)
    {
        if (short_name || !long_name.empty())
        {
            auto key = hash<string_view>{}(prop_name);
            m_properties.emplace(key, prop_name);
            m_entries.emplace(prop_name, option_entry{ short_name, (string_t)long_name, (string_t)help_text });
            if (short_name) m_short_args.emplace(short_name, key);
            if (!long_name.empty()) m_long_args.emplace(long_name, key);
        }
        else
        {
            m_default_property = prop_name;
        }
        return static_pointer_cast<option>(shared_from_this());
    }

    basic_ostream<char_t>& option::print_help(basic_ostream<char_t>& stream) const
    {
        constexpr size_t offset = 2;
        constexpr size_t spacing = 24;
        for (auto& prop : m_entries)
        {
            size_t count = offset;
            stream << string_t(offset, U(' '));
            auto& entry = prop.second;
            if (entry.short_arg)
            {
                stream << '-' << entry.short_arg;
                count += 2;
            }
            if (!entry.long_arg.empty())
            {
                if (entry.short_arg)
                {
                    stream << ", ";
                    count += 2;
                }
                else
                {
                    stream << string_t(4, U(' '));
                    count += 4;
                }
                stream << "--" << entry.long_arg;
                count += 2 + entry.long_arg.length();
            }
            if (!entry.short_arg && entry.long_arg.empty())
            {
                stream << "[default]";
                count += 9;
            }
            if (count < spacing)
            {
                stream << string_t(spacing - count, U(' '));
            }
            stream << entry.help_text;
            stream << endl;
        }
        return stream;
    }
} // namespace xaml::cmdline
