#include <xaml/cmdline/option.h>
#include <xaml/map.h>

using namespace std;

#define m_outer_this this

struct xaml_cmdline_option_entry_impl : xaml_implement<xaml_cmdline_option_entry_impl, xaml_cmdline_option_entry, xaml_object>
{
    XAML_PROP_IMPL(short_arg, xaml_char_t, xaml_char_t*, xaml_char_t)
    XAML_PROP_PTR_IMPL(long_arg, xaml_string)
    XAML_PROP_PTR_IMPL(help_text, xaml_string)
};

#undef m_outer_this

struct xaml_cmdline_option_impl : xaml_implement<xaml_cmdline_option_impl, xaml_cmdline_option, xaml_object>
{
    xaml_ptr<xaml_map> m_short_args;
    xaml_ptr<xaml_map> m_long_args;
    xaml_ptr<xaml_map> m_entries;
    xaml_ptr<xaml_string> m_default_prop;

    xaml_result XAML_CALL init() noexcept;

    xaml_result XAML_CALL find_short_arg(xaml_char_t name, xaml_string** ptr) noexcept override;
    xaml_result XAML_CALL find_long_arg(xaml_string* name, xaml_string** ptr) noexcept override;
    xaml_result XAML_CALL get_default_property(xaml_string** ptr) noexcept override { return m_default_prop->query(ptr); }
    xaml_result XAML_CALL get_entries(xaml_map_view** ptr) noexcept override { return m_entries->query(ptr); }
    xaml_result XAML_CALL add_arg(xaml_char_t, xaml_string*, xaml_string*, xaml_string*) noexcept override;
};

xaml_result xaml_cmdline_option_impl::init() noexcept
{
    xaml_ptr<xaml_hasher> char_hasher;
    XAML_RETURN_IF_FAILED(xaml_hasher_new<xaml_char_t>(&char_hasher));
    XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(char_hasher.get(), &m_short_args));
    xaml_ptr<xaml_hasher> string_hasher;
    XAML_RETURN_IF_FAILED(xaml_hasher_string_default(&string_hasher));
    XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_long_args));
    XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_entries));
    return XAML_S_OK;
}

xaml_result xaml_cmdline_option_impl::find_short_arg(xaml_char_t name, xaml_string** ptr) noexcept
{
    xaml_ptr<xaml_object> key;
    XAML_RETURN_IF_FAILED(xaml_box_value(name, &key));
    xaml_ptr<xaml_object> item;
    XAML_RETURN_IF_FAILED(m_short_args->lookup(key.get(), &item));
    return item->query(ptr);
}

xaml_result xaml_cmdline_option_impl::find_long_arg(xaml_string* name, xaml_string** ptr) noexcept
{
    xaml_ptr<xaml_object> key;
    XAML_RETURN_IF_FAILED(xaml_box_value(name, &key));
    xaml_ptr<xaml_object> item;
    XAML_RETURN_IF_FAILED(m_short_args->lookup(key.get(), &item));
    return item->query(ptr);
}

xaml_result xaml_cmdline_option_impl::add_arg(xaml_char_t short_name, xaml_string* long_name, xaml_string* prop_name, xaml_string* help_text) noexcept
{
    if (short_name || !long_name)
    {
        bool replaced;
        xaml_ptr<xaml_cmdline_option_entry> entry;
        XAML_RETURN_IF_FAILED(xaml_object_new<xaml_cmdline_option_entry_impl>(&entry));
        if (short_name)
        {
            xaml_ptr<xaml_object> key;
            XAML_RETURN_IF_FAILED(xaml_box_value(short_name, &key));
            XAML_RETURN_IF_FAILED(m_short_args->insert(key.get(), prop_name, &replaced));
            XAML_RETURN_IF_FAILED(entry->set_short_arg(short_name));
        }
        if (long_name)
        {
            XAML_RETURN_IF_FAILED(m_long_args->insert(long_name, prop_name, &replaced));
            XAML_RETURN_IF_FAILED(entry->set_long_arg(long_name));
        }
        if (help_text)
        {
            XAML_RETURN_IF_FAILED(entry->set_help_text(help_text));
        }
        XAML_RETURN_IF_FAILED(m_entries->insert(prop_name, entry.get(), &replaced));
    }
    else
    {
        m_default_prop = prop_name;
    }
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_cmdline_option_new(xaml_cmdline_option** ptr) noexcept
{
    return xaml_object_init<xaml_cmdline_option_entry_impl>(ptr);
}

xaml_result XAML_CALL xaml_cmdline_option_print(FILE*, xaml_cmdline_option* opt) noexcept
{
    return XAML_E_NOTIMPL;
}

xaml_result XAML_CALL xaml_cmdline_option_print(basic_ostream<xaml_char_t>& stream, xaml_cmdline_option* opt) noexcept
{
    constexpr int32_t offset = 2;
    constexpr int32_t spacing = 24;
    for (auto& prop : m_entries)
    {
        int32_t count = offset;
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
    return XAML_S_OK;
}
