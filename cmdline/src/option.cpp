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

#ifdef UNICODE
#define _tfprintf fwprintf
#else
#define _tfprintf fprintf
#endif // UNICODE

static inline void xaml_print(FILE* stream, size_t length) noexcept
{
    _tfprintf(stream, U("%*s"), (int)length, U(""));
}

static inline void xaml_print(basic_ostream<xaml_char_t>& stream, size_t length) noexcept
{
    stream << xaml_std_string_t(length, U(' '));
}

static inline void xaml_print(FILE* stream, xaml_char_t const* str) noexcept
{
    _tfprintf(stream, U("%s"), str);
}

static inline void xaml_print(basic_ostream<xaml_char_t>& stream, xaml_char_t const* str) noexcept
{
    stream << str;
}

static inline void xaml_print(FILE* stream, xaml_char_t c) noexcept
{
    _tfprintf(stream, U("%c"), c);
}

static inline void xaml_print(basic_ostream<xaml_char_t>& stream, xaml_char_t c) noexcept
{
    stream << c;
}

static inline void xaml_println(FILE* stream) noexcept
{
    _tfprintf(stream, U("\n"));
}

static inline void xaml_println(basic_ostream<xaml_char_t>& stream) noexcept
{
    stream << endl;
}

template <typename Stream>
xaml_result XAML_CALL xaml_cmdline_option_print_impl(Stream&& stream, xaml_cmdline_option* opt) noexcept
{
    constexpr size_t offset = 2;
    constexpr size_t spacing = 24;
    xaml_ptr<xaml_map_view> entries;
    XAML_RETURN_IF_FAILED(opt->get_entries(&entries));
    XAML_FOREACH_START(p, entries);
    {
        xaml_ptr<xaml_key_value_pair> prop;
        XAML_RETURN_IF_FAILED(p->query(&prop));
        size_t count = offset;
        xaml_print(stream, offset);
        xaml_ptr<xaml_object> value;
        XAML_RETURN_IF_FAILED(prop->get_value(&value));
        xaml_ptr<xaml_cmdline_option_entry> entry;
        XAML_RETURN_IF_FAILED(value->query(&entry));
        xaml_char_t short_arg;
        XAML_RETURN_IF_FAILED(entry->get_short_arg(&short_arg));
        if (short_arg)
        {
            xaml_print(stream, U("-"));
            xaml_print(stream, short_arg);
            count += 2;
        }
        xaml_ptr<xaml_string> long_arg;
        XAML_RETURN_IF_FAILED(entry->get_long_arg(&long_arg));
        int32_t long_arg_length = 0;
        if (long_arg)
            XAML_RETURN_IF_FAILED(long_arg->get_length(&long_arg_length));
        if (long_arg_length)
        {
            if (short_arg)
            {
                xaml_print(stream, U(", "));
                count += 2;
            }
            else
            {
                xaml_print(stream, size_t(4));
                count += 4;
            }
            xaml_char_t const* data;
            XAML_RETURN_IF_FAILED(long_arg->get_data(&data));
            xaml_print(stream, U("--"));
            xaml_print(stream, data);
            count += 2 + long_arg_length;
        }
        if (!short_arg && !long_arg_length)
        {
            xaml_print(stream, U("[default]"));
            count += 9;
        }
        if (count < spacing)
        {
            xaml_print(stream, spacing - count);
        }
        {
            xaml_ptr<xaml_string> help_text;
            XAML_RETURN_IF_FAILED(entry->get_help_text(&help_text));
            if (help_text)
            {
                xaml_char_t const* help_data;
                XAML_RETURN_IF_FAILED(help_text->get_data(&help_data));
                xaml_print(stream, help_data);
            }
        }
        xaml_println(stream);
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_cmdline_option_print(FILE* stream, xaml_cmdline_option* opt) noexcept
{
    return xaml_cmdline_option_print_impl(stream, opt);
}

xaml_result XAML_CALL xaml_cmdline_option_print(basic_ostream<xaml_char_t>& stream, xaml_cmdline_option* opt) noexcept
{
    return xaml_cmdline_option_print_impl(stream, opt);
}
